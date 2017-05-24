<?php

namespace App\Http\Controllers;

use \App\Content;
use Illuminate\Http\Request;

class ContentController extends Controller {
	private $rules = [
		'file'         => 'required|image',
		'foreign_id'   => 'required|numeric',
		'foreign_type' => 'required',
	];

	/**
	 * Display a listing of the resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function index () {
		if (True || \Entrust::can('view_content')) {
			$resp   = Content::all();
			$status = 200;
		} else {
			$resp   = 'unauthorized access';
			$status = 403;
		}

		return response()->json([
			'status'   => $status,
			'response' => $resp,
		], $status);
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
		if (True || \Entrust::can('manage_content')) {
			$this->validate($request, $rules);

			$resp   = Content::create($request->all());
			$status = 200;
		} else {
			$status = 403;
			$resp   = 'unauthorized create request';
		}

		return response()->json([
			'status'   => $status,
			'response' => $resp,
		], $status);
	}

	public function createFor ($contentsArray, $id, $type) {
		if (True || \Entrust::can('manage_content')) {
			if (!array_filter($contentsArray, 'is_array')) {
				$contentsArray = [$contentsArray];
			}

			$resp = [];
			foreach ($contentsArray as $content) {
				$content['foreign_id']   = $id;
				$content['foreign_type'] = $type;
				$content['content_path'] = $content['file']->store('images');
				$content['content_type'] = 'image';

				\Validator::make($content, $rules)->validate();

				$resp[] = Content::create($content);
			}

			return $resp;
		}
	}

	/**
	 * Display the specified resource.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function show ($id) {
		if (True || \Entrust::can('view_content')) {
			$resp   = Content::findOrFail($id);
			$status = 200;
		} else {
			$status = 403;
			$resp   = 'unauthorized access';
		}

		return response()->json([
			'status'   => $status,
			'response' => $resp,
		], $status);
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
		if (True || \Entrust::can('manage_content')) {
			$content = Content::findOrFail($id);

			$path = $content->content_path;

			$resp   = \Storage::delete($path) && $content->delete();
			$status = 200;
		} else {
			$status = 403;
			$resp   = 'unauthorized access';
		}

		return response()->json([
			'status'   => $status,
			'response' => $resp,
		], $status);
	}
}
