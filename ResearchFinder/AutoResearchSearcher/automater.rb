#! /usr/bin/ruby

## ############################################################ ##
##  A script used to automatically search on Google Scholar     ##
##  for researches made by certain users, it is intended to     ##
##  be run as a cronjob to periodically update some database    ##
##  using a following script/program.                           ##
## ############################################################ ##
## ########## LICENSED # UNDER # THE # MIT # LICENSE ########## ##
## ############################################################ ##

VERSION_NUMBER = '1.0.1a'

require 'optparse'
require 'open-uri'
require 'logger'


# place website names here, default to parsing the title tag or going to google for other stuff
$site_names                        = Hash.new nil
$site_names['ieeexplore.ieee.org'] = 'IEEEXplore'
$site_names['dl.acm.org']          = 'ACM Digital Library'
$site_names['www.igi-global.com']  = 'IGI Global'
$site_names['link.springer.com']   = 'Springer'
$site_names['springer.com']        = 'Springer'


# Main functionality of the program, parses a block of an HTML page producing a Hash map with the results per person
# @param params Hash contains the parameters used by the function,
#       :results containing the HTML block with the results,
#       :check_re (optional) containing the regular expression for validation
# @return returns an Array containing Hash tables holding the data of each result
def parse_results (params)
	# Parses results and returns an array for each result as JSON
	# Function parameters
	results  = params[:results]
	check_re = (params[:check_re] or /(.*)/)

	ret = []

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
			# get the section containing the pdf info and link
			# remove beginning div tag and trailing div from next
			pdf_div  = result.match(/<div class="gs_ggs gs_fl">(.*)<div class="gs_ri">/)
			# no guarantee for pdf div from Google
			pdf_link = nil
			unless pdf_div.nil?
				pdf_div  = pdf_div[0].gsub(/(<div class="gs_ggs gs_fl">|<div class="gs_ri">$)/, '')
				pdf_link = pdf_div.match(/href="([^"]*)"/)[0].gsub(/(^href="|"$)/, '')
			end

			# this variable contains the link to the PDF from Google Scholar, (regardless paid or not)
			# this one hold the rest of the data as HTML content
			inf_div           = result.match(/<div class="gs_ri">(.*)$/)[0].gsub(/^<div class="gs_ri">/, '')

			# title of paper as well as link to it
			title_link        = inf_div.match(/<h\d(.*)<\/h\d>/)[0].gsub(/^<h\d(.*)><a|<\/h\d>$/, '')
			publication_link  = title_link.match(/href="([^"]*)"/)[0].gsub(/(^href="|"$)/, '')
			publication_title = title_link.match(/>(.*)<\/a>/)[0].gsub(/(^>|<\/a>$)/, '')
			# publisher         = publication_link.match(/(.*)([.])?(.*)[.](.*)[\/]/)[0].strip
			# authors of it, also contains the link of the publisher as well as a tag of some sort referring to the topic
			authors_div_arr   = inf_div.match(/<div class="gs_a">(.*)<div class="gs_rs">/)[0].gsub(/^<div class="gs_a">|<div class="gs_rs">$/, '').gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').split('-')
			# abstract extracted by Google Scholar
			abstract_div      = inf_div.match(/<div class="gs_rs">(.*)<div class="gs_fl">/)[0].gsub(/^<div class="gs_rs">|<div class="gs_fl">$/, '')

			# dictionary holding the data
			res               = Hash.new '__null__'

			res['__title__']     = publication_title.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			res['__link__']      = publication_link.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			res['__authors__']   = authors_div_arr[0].strip # gsub(/"/, '\"').gsub(/<([^<>]*)>/, '')
			res['__abstract__']  = abstract_div.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			res['__publisher__'] = ($site_names[authors_div_arr[-1].strip] or authors_div_arr[-1]).strip # .gsub(/"/, '\"').gsub(/<([^<>]*)>/, '')
			unless pdf_link.nil?
				res['__pdf__'] = pdf_link.gsub(/"/, '\"').gsub(/<([^<>]*)>/, '').strip
			end

			# append to return variable
			ret << res
		end
	end

	ret
end

#####################
## Program globals ##
#####################
# Google Scholar link
# In code adds the query after `q=`
# add `&num=20` after string to get the max number of results per page Scholar gives.
BASE_URL    = 'https://scholar.google.com/scholar?q='
# default path for input, output, and logging files
INPUT_FILE  = './input.users'
OUTPUT_FILE = './output.results'
LOGGER      = './logger'
CROSS_CHECK = true

#####################################
## HANDLING OPTIONS OF THE PROGRAM ##
#####################################
OptionParser.new do |options|
	options.banner = 'Usage: automater.rb [-i<input file> -o<output file> -l<logger file> -noc]'

	options.on('-i', '--input INPUT', 'Specify input file ... Default is "./input.users"') do |input|
		INPUT_FILE = input
	end

	options.on('-o', '--output OUTPUT', 'Specify output file ... Default is "./output.users"') do |out|
		OUTPUT_FILE = out
	end

	options.on('-l', '--logger LOGGER', 'Specify logging file ... Default is "./logger"') do |logf|
		LOGGER = logf
	end

	options.on('-v', '--version', 'Print version and exit ..') do
		p "#{VERSION_NUMBER}"
		exit 0
	end

	options.on('-noc' '--NOCHECK-USING-LASTNAME', 'To disable the use of the last name as a RegEx based to validate result or not') do
		CROSS_CHECK = false
	end
end.parse!
logging = Logger.new open(LOGGER, 'a') # Append to old log file instead of overwriting it


####################
## MAIN CODE BODY ##
####################
begin
	exit_status = 0
	logging.info "#{Time.now} -> Began executing ..."

	# Read users names, if failed exit
	input  = open(INPUT_FILE, 'r')
	output = open(OUTPUT_FILE, 'w')

	logging.info 'Opened files ...'

	# Data to be written to output file
	out = ''
	# Foreach user, search on Google Scholar, parse results of the first page and write them to output file
	input.each_line do |line|
		unless line =~ /^\s*[\{\}]?\s*$/
			info = line.split(':')
			id   = info[0].strip.gsub(/["']/, ''); user = info[1].strip.gsub(/(^["']|["']\s*(,)?\s*$)/, '').strip

			logging.info "Processing user #{user} of id #{id} ..."

			open(BASE_URL + URI::escape(user)) do |page|
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
				out << "\n\t\"#{id}\": {\n\t\t\"__user__\" : \"#{user}\",\n\t\t\"__researches__\" : #{if results != [] then '[' else 'null' end}"

				# if results returned aren't an empty set, then do processing
				if results != []
					# remove HTML tags requires extra libraries
					# for simplicity that is left for the PHP side of the code
					results.each do |res|
						# made into one liner to avoid breaking the formatting in file when refactoring the code in IDEs
						# apologies to anyone trying to decipher this lol
						out << "\n\t\t\t{\n\t\t\t\t\"__title__\" : \"#{res['__title__']}\",\n\t\t\t\t\"__url__\" : \"#{res['__link__']}\",\n\t\t\t\t\"__authors__\" : \"#{res['__authors__']}\",\n\t\t\t\t\"__abstract__\" : \"#{res['__abstract__']}\",\n\t\t\t\t\"__publisher__\" : \"#{res['__publisher__']}\",\n\t\t\t\t\"__pdf__\" : \"#{res['__pdf__']}\"\n\t\t\t},"
					end

					# remove trailing comma
					# remove quotes around any defaulted null string
					out = out[0...-1].gsub(/["]__null__["]/, 'null')
					out << "\n\t\t]"
				end

				out << "\n\t},"
				logging.info "Done processing #{user} of id #{id} ... no issues"
			end # END of website processing
			# write data to output file, ignoring trailing comma
		end # END of line processing
	end # END of input file processing
	# Write to output file, avoid the ignore comma from the last iteration
	output.write "{\n#{out[0...-1]}\n}\n"
rescue Exception => e
	logging.error "#{e.message}, #{e.backtrace}"
	exit_status = -1
ensure
	# Close files
	input.close
	output.close

	logging.info "Exiting program status #{exit_status}"

	# Exit program
	exit exit_status
end # END of program
