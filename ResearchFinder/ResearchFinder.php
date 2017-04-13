<?php

/**
 * Plugin Name: Research Finder
 * Description: A Plugin that takes the users in roles related to research, then starts an external Ruby script to find the researches of those users in Google Scholar, then adds those researches as a custom type (supposed to already exist) named Research.
 * Version: 0.1.1
 * Author: oji
 * Author URI: https://github.com/ojizero/
 * License: MIT
 */

$PLUGIN_PATH      = plugin_dir_path(__FILE__);
$RUBY_SCRIPT      = $PLUGIN_PATH . 'AutoResearchSearcher/automater.rb';
$OUTPUT_PATH      = $PLUGIN_PATH . 'TmpData/ruby_input';
$INPUT_PATH       = $PLUGIN_PATH . 'TmpData/ruby_output';
$LOGGER_PATH      = $PLUGIN_PATH . 'TmpData/ruby_log';
$RESEARCHER_TYPES = [
	'assistant',
	'mastersdirector',
	'facultymember',
	'chairperson',
	'assistnatprofessor',
];


class ResearchFinder {

	public function __construct () {

	}


	###############################
	# Plugin's main functionality #
	###############################

	/**
	 * Reads user data from the database, prints users that make researches, writes data to external file named as the $OUTPUT_PATH global variable
	 **/
	public function prepare_data () {
		GLOBAL $RESEARCHER_TYPES, $OUTPUT_PATH;

		$query = new WP_User_Query(
			['role__in' => $RESEARCHER_TYPES]
		);
		$users = $query->get_results();

		$output = fopen($OUTPUT_PATH, 'w');
		fwrite($output, '{\n');

		$len = count($users);
		for ($index = 0; $index < $len; $index++):
			fwrite($output, $users[$index]->ID . ' : "' . $users[$index]->first_name . ' ' . $users[$index]->last_name . '"' . ($index == $len - 1) ? (',') : ('') . '\n');
		endfor;

		fwrite($output, '}\n');
		fclose($output);
	}

	/**
	 * Makes a system call for the ruby script to process the users data and retrieve researches from the web
	 **/
	public function call_finder () {
		GLOBAL $RUBY_SCRIPT, $OUTPUT_PATH, $INPUT_PATH, $LOGGER_PATH;
		system("ruby $RUBY_SCRIPT -i$OUTPUT_PATH -o$INPUT_PATH -l$LOGGER_PATH");
	}

	/**
	 * Retrieves the results of the Ruby script, and adds them to the database properly
	 **/
	public function retrieve_results () {
		GLOBAL $INPUT_PATH, $wpdb;

		$data_file = fopen($INPUT_PATH, 'r');
		$data      = json_decode(fread($data_file, filesize($INPUT_PATH)), true);
		fclose($data_file);

		foreach ($data as $uid => $udata):
			$uname = strip_tags($udata['__user__']);

			foreach ($udata['__researches__'] as $research_data):
				$meta_info = [
					'wpcf-research-title'   => strip_tags($research_data['__title__']),
					'wpcf-research'         => strip_tags($research_data['__pdf__']),
					'wpcf-publication-date' => '' . time(),
					'wpcf-publishedin'      => strip_tags($research_data['__publisher__']),
					'wpcf-publication-url'  => strip_tags($research_data['__url__']),
					'wpcf-abstract'         => strip_tags($research_data['__abstract__']),
				];
				$post_info = [
					'ID'          => 0, # Let auto_increment do its thing
					'post_author' => intval($uid),
					'post_title'  => strip_tags("Research: {$research_data['__title__']}"),
					'post_status' => 'pending',
					'post_type'   => 'research',
					'meta_input'  => $meta_info,
				];

				$post_id = wp_insert_post($post_info);
				foreach (preg_split('/,/', $research_data['__authors__']) as $author_name):
					$wpdb->insert(
						'wp_postmeta',
						[
							'post_id'    => $post_id,
							'meta_key'   => 'wpcf-author',
							'meta_value' => $author_name
						],
						[
							'%d',
							'%s',
							'%s'
						]
					);
				endforeach;
			endforeach;

			add_user_meta(intval($uid), '__has_pending_research__', 'true');
		endforeach;
	}


}
