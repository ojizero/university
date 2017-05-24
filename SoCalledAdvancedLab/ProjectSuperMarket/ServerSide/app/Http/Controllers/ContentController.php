<?php

namespace App\Http\Controllers;

use \App\Content;
use Illuminate\Http\Request;

class ContentController extends Controller {
	/**
	 * Display a listing of the resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function index () {
		//
	}

	/**
	 * Show the form for creating a new resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function create () {
		//
	}

	/**
	 * Store a newly created resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @return \Illuminate\Http\Response
	 */
	public function store (Request $request) {
		//
	}

	public function createFor ($contentsArray, $id, $type) {
		if (!array_filter($contentsArray, 'is_array')) {
			$contentsArray = [$contentsArray];
		}

		$resp = [];
		foreach ($contentsArray as $content) {
			$content['foreign_id']   = $id;
			$content['foreign_type'] = $type;
			$content['content_path'] = $content['file']->store('images');
			$content['content_type'] = 'image';

			$resp[] = Content::create($content);
		}

		return $resp;
	}

	/**
	 * Display the specified resource.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function show ($id) {
		//
	}

	/**
	 * Show the form for editing the specified resource.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function edit ($id) {
		//
	}

	/**
	 * Update the specified resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function update (Request $request, $id) {
		//
	}

	/**
	 * Remove the specified resource from storage.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function destroy ($id) {
		//
	}
}
