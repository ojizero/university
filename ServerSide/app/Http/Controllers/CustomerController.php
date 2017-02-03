<?php

namespace App\Http\Controllers;


use App\CustomerClient;


class CustomerController extends Controller {
	public static function deposit_money ($uid, $amount) {
		$customer = CustomerClient::find($uid);
		if ($customer) {
			$customer->credit = $customer->credit + $amount;
			$customer->update();

			return response()->json(True);
		}

		return response()->json(False);
	}

	public static function withdraw_money ($uid, $amount) {
		$customer = CustomerClient::find($uid);
		if ($customer && $customer->credit >= $amount) {
			$customer->credit = $customer->credit - $amount;
			$customer->update();

			return response()->json(True);
		}

		return response()->json(False);
	}

	public static function get_credit ($id) {
		$customer = CustomerClient::find($id);
		if ($customer) {
			return response()->json($customer->credit);
		}

		return response()->json(Null);
	}
}
