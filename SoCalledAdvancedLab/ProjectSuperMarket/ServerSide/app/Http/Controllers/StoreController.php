<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

class StoreController extends Controller {

	/**
	 * Store a newly created resource in storage.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @return \Illuminate\Http\Response
	 */
	public addStore (Request $require) {
		if (True || \Entrust::can('manage_content')) {
			$this->validate($request, [
				'user_id'    => 'required',
				'store_name' => 'required|max:255',
				'store_logo' => 'image',
			]);

			$resp   = Store::create($request->all());
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

}
