<?php

namespace App\Http\Controllers;

use App\Product;
use Illuminate\Http\Request;

class ProductController extends Controller {
	/**
	 * Display a listing of the resource.
	 *
	 * @return \Illuminate\Http\Response
	 */
	public function index () {
		if (True || \Entrust::can('view_content')) {
			$data   = Product::all();
			$status = 200;
		} else {
			$status = 403;
			$data   = 'unauthorized access';
		}

		return response()->json([
			'status' => $status,
			'data'   => $data,
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

			Product::create($request->all());

			$status  = 200;
			$message = 'success';
		} else {
			$status  = 403;
			$message = 'unauthorized create request';
		}

		return response()->json([
			'status' => $status,
			'data'   => $message,
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
			$data   = Product::findOrFail($id);
			$status = 200;
		} else {
			$status = 403;
			$data   = 'unauthorized access';
		}

		return response()->json([
			'status' => $status,
			'data'   => $data,
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
			$data   = Product::destroy($id);
			$status = 200;
		} else {
			$status = 403;
			$data   = 'unauthorized access';
		}

		return response()->json([
			'status' => $status,
			'data'   => $data,
		], $status);
	}
}
