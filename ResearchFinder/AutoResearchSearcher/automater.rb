#! /usr/bin/ruby

## ############################################################ ##
##  A script used to automatically search on Google Scholar     ##
##  for researches made by certain users, it is intended to     ##
##  be run as a cronjob to periodically update some database    ##
##  using a following script/program.                           ##
## ############################################################ ##
## ############# #LICENSED UNDER THE #MIT LICENSE ############# ##
## ############################################################ ##

VERSION_NUMBER = '1.0.2'

require 'digest/sha2'
require 'optparse'
require 'open-uri'
require 'logger'
require 'json'
require 'set'

#####################
## Program globals ##
#####################
# Google Scholar link
# FIXME this isn't allowed according to the Google `robots.txt` so the script is being blocked
# In code adds the query after `q=`
# add `&num=20` after string to get the max number of results per page Scholar gives.
BASE_URL                           = 'https://scholar.google.com/scholar?q='
# default path for input, output, and logging files
INPUT_FILE                         = './input.json'
OUTPUT_FILE                        = './output.json'
LOGGER                             = './logger'
CROSS_CHECK                        = true
USE_INDEX                          = false
INDEX                              = nil
INDEX_FILE                         = nil
FAKE_USER_AGENT                    = false
FIREFOX_USER_AGENT                 = 'Mozilla/5.0 (X11; Linux x86_64; rv:49.0) Gecko/20100101 Firefox/49.0'
RUBY_USER_AGENT                    = 'Ruby'

# place website names here, default to parsing the title tag or going to google for other stuff
$SITE_NAMES                        = Hash.new nil
$SITE_NAMES['ieeexplore.ieee.org'] = 'IEEEXplore'
$SITE_NAMES['dl.acm.org']          = 'ACM Digital Library'
$SITE_NAMES['www.igi-global.com']  = 'IGI Global'
$SITE_NAMES['link.springer.com']   = 'Springer'
$SITE_NAMES['springer.com']        = 'Springer'


# Main functionality of the program, parses a block of an HTML page producing a Hash map with the results per person
# @param [Hash] params contains the parameters used by the function,
#   @option params [String] :results     containing the HTML block with the results,
#   @option params [String] :check_re    (optional) containing the regular expression for validation
# @return [Array<Hash>, Set<String>] returns an Array containing Hash tables holding the data of each result as a first element, and the Sha2 of the titles of the researches if USE_INDEX is enabled (else nil:NilClass)
def parse_results (params)
	# Parses results and returns an array for each result as JSON
	# Function parameters
	results  = params[:results]
	check_re = (params[:check_re] or /(.*)/)

	newly_added = Set.new
	ret         = []

	results = results.gsub(/<\/div>|<div id="gs_ccl_results">/, '').strip # remove closing divs

	results.split(/<div class="gs_r">/).each do |result|
		# IGNORE result
		# if it's empty
		# OR
		# if result is user's home page (not paper)
		# OR
		# if result is a user citation
		# OR
		# if it's not validated by the string
		unless result == '' or result =~ /(.*(user profiles for).*|\[citation\])/i or (not result =~ check_re)
			# this variable contains the link to the PDF from Google Scholar, (regardless paid or not)
			# this one hold the rest of the data as HTML content
			inf_div           = result.match(/<div class="gs_ri">(.*)$/)[0].gsub(/^<div class="gs_ri">/, '')

			# title of paper as well as link to it
			title_link        = inf_div.match(/<h\d(.*)<\/h\d>/)[0].gsub(/^<h\d(.*)><a|<\/h\d>$/, '')
			publication_title = title_link.match(/>(.*)<\/a>/)[0].gsub(/(^>|<\/a>$)/, '')
			title_sha2        = Digest::SHA2.hexdigest(publication_title)
			# if title is already indexed, or was visitied before in the current parse then skip
			if USE_INDEX and INDEX.include? (title_sha2 + "\n") and newly_added.include? title_sha2
				next
			end

			publication_link = title_link.match(/href="([^"]*)"/)[0].gsub(/(^href="|"$)/, '')
			# authors of it, also contains the link of the publisher as well as a tag of some sort referring to the topic
			authors_div_arr  = inf_div.match(/<div class="gs_a">(.*)<div class="gs_rs">/)[0].gsub(/^<div class="gs_a">|<div class="gs_rs">$/, '').gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').split('-')
			# abstract extracted by Google Scholar
			abstract_div     = inf_div.match(/<div class="gs_rs">(.*)<div class="gs_fl">/)[0].gsub(/^<div class="gs_rs">|<div class="gs_fl">$/, '')

			# get the section containing the pdf info and link
			# remove beginning div tag and trailing div from next
			pdf_div          = result.match(/<div class="gs_ggs gs_fl">(.*)<div class="gs_ri">/)
			# no guarantee for pdf div from Google
			pdf_link         = nil
			unless pdf_div.nil?
				pdf_div  = pdf_div[0].gsub(/(<div class="gs_ggs gs_fl">|<div class="gs_ri">$)/, '')
				pdf_link = pdf_div.match(/href="([^"]*)"/)[0].gsub(/(^href="|"$)/, '')
			end


			# dictionary holding the data
			res = {}

			res[:__title__]     = publication_title.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			res[:__link__]      = publication_link.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			res[:__authors__]   = authors_div_arr[0].strip
			res[:__abstract__]  = abstract_div.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			res[:__publisher__] = ($SITE_NAMES[authors_div_arr[-1].strip] or authors_div_arr[-1]).strip
			res[:__hash__]      = title_sha2
			pdf_link.nil? ? res[:__pdf__] = nil : res[:__pdf__] = pdf_link.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip

			# append to return variable
			ret << res
			newly_added << res[:__hash__] if USE_INDEX
		end
	end

	return ret, newly_added
end

#####################################
## HANDLING OPTIONS OF THE PROGRAM ##
#####################################
OptionParser.new do |options|
	options.banner = 'Usage: automater.rb [-i<input file> -o<output file> -l<logger file> -n<indexing file> -f -u]'

	options.on('-i', '--input INPUT', 'Specify input file ... Default is "./input.json"') do |input|
		INPUT_FILE = input
	end

	options.on('-o', '--output OUTPUT', 'Specify output file ... Default is "./output.users"') do |out|
		OUTPUT_FILE = out
	end

	options.on('-l', '--logger LOGGER', 'Specify logging file ... Default is "./logger"') do |logf|
		LOGGER = logf
	end

	options.on('-v', '--version', 'Print version and exit ..') do
		puts VERSION_NUMBER
		exit 0
	end

	# FIXME weird error here ?!
	options.on('-u' '--no-check-last', 'To disable the use of the last name as a RegEx based to validate result or not ... Default true') do
		CROSS_CHECK = false
	end

	options.on('-n' '--USE_INDEX INDEX_FILE', 'Whether or not to use an index file to avoid sending data already existent in the database ... Default no index file') do |index_file|
		f     = open(index_file, 'r')
		INDEX = Set.new(f.readlines)
		f.close

		USE_INDEX  = true
		INDEX_FILE = open(index_file, 'a')
	end

	options.on('-f', '--fake-user-agent', 'Fakes the User-Agent field in the HTTP request header as if it was Firefox browser ... Default is false') do
		FAKE_USER_AGENT = true
	end
end.parse!
# Append to old log file instead of overwriting it
logging = Logger.new open(LOGGER, 'a')


####################
## MAIN CODE BODY ##
####################
begin
	exit_status = 0
	logging.info "#{Time.now} -> Began executing ..."

	# Read users names, if failed exit
	f      = open(INPUT_FILE, 'r')
	input  = JSON.parse(f.readlines.join)
	output = open(OUTPUT_FILE, 'w')
	f.close

	logging.info 'Prepared files ...'

	# Data to be written to output file
	out = {}

	# Foreach user, search on Google Scholar, parse results of the first page and write them to output file
	input.each do |id, user|
		user.strip!

		logging.info "Processing user #{user} of id #{id} ..."

		begin
			user_search_query = BASE_URL + URI::escape(user)
			user_agent        = FAKE_USER_AGENT ? FIREFOX_USER_AGENT : RUBY_USER_AGENT

			open(user_search_query, 'User-Agent' => user_agent) do |page|
				# get document by reading the page
				document = page.readlines.join("\n")

				if document =~ /(.*)did not match any articles(.*)/i
					logging.info "Done processing #{user} of id #{id} ... no results found ... skipping"
					break
				end

				# from document body, get only the results <div>
				results = document.match(/<div\s+id="gs_ccl_results">(.*)<div\s+id="gs_ccl_bottom">/)
				if not results.nil?
					results = results[0]
				else
					logging.info "Done processing #{user} of id #{id} ... results was `nil` ... avoided error by skipping"
					break
				end
				results = results.sub(/<div id="gs_ccl_bottom">/, '').strip

				# parameters of the parsing function
				params  = {:results => results}
				if CROSS_CHECK
					params[:check_re] = /(.*)#{user.split(' ')[-1]}(.*)/i
				end
				# parse the results, for specified user as to avoid any misidentification
				results = parse_results(params)

				# prepare what's to be written to output file
				out[id] = {
					:__user__       => user,
					:__researches__ => results[0]
				}

				# convert set to array, then to string, then replace ',' with LF and remove brackets
				INDEX_FILE.write results[1].to_a.to_s.gsub(/, /, "\n").gsub(/[\["']/, '').gsub(/\]/, "\n") if USE_INDEX
				logging.info "Done processing #{user} of id #{id} ... no issues"
			end # END of website processing for user
		rescue HTTPServiceUnavailable => h
			logging.error "HTTP 503 response from `#{user_search_query}` on user `#{user}`.\nMost probably cause is Google enforcing their `robots.txt` instructions via blocking. Possible (though not guaranteed) solution would be running this script with the option -f (--fake-user-agent) enabled if not already, to fake the User-Agent in header as if it was a Firefox browser.\n#{h.message}\n\nMoving on."
			logging.info "Done processing #{user} of id #{id} ... no results retrieved due to HTTP503 response"
		end
	end # END of processing
rescue Exception => e
	logging.error "#{e.message}, #{e.backtrace}"
	exit_status = -1
ensure
	# Write to output file
	output.write JSON.pretty_generate(out)
	# Close files
	output.close
	INDEX_FILE.close unless INDEX_FILE.nil?

	logging.info "Exiting program status #{exit_status}"

	# Exit program
	exit exit_status
end # END of program
