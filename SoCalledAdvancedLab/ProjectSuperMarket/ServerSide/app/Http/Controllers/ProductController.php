<?php

namespace App\Http\Controllers;

use App\Product;
use Illuminate\Http\Request;

class ProductController extends Controller {

	public function __construct () {
		$this->middleware('content');
	}

	/**
	 * Display a listing of the resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function index () {
		if (True || \Entrust::can('view_content')) {
			$resp   = Product::all();
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
			$this->validate($request, [
				'product_name'  => 'required|max:255',
				'product_price' => 'required|numeric|min:1',
				'availability'  => 'required|boolean',
			]);

			$resp   = [
				'product' => Product::create($request->all()),
				'content' => $request['_content_result'],
			];
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

	/**
	 * Display the specified resource.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function show ($id) {
		if (True || \Entrust::can('view_content')) {
			$resp   = Product::findOrFail($id);
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
		if (True || \Entrust::can('manage_content')) {
			$resp   = Product::findOrFail($id)->update($request->all());
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
	 * Remove the specified resource from storage.
	 *
	 * @param  int  $id
	 * @return \Illuminate\Http\Response
	 */
	public function destroy ($id) {
		if (True || \Entrust::can('manage_content')) {
			$resp   = Product::destroy($id);
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
