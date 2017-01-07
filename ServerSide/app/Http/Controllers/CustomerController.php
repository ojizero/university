<?php

namespace App\Http\Controllers;


use App\CustomerClient;


class CustomerController extends Controller {
	public static function deposit_money ($id, $amount) {
		$customer = CustomerClient::find($id);
		if ($customer) {
			$customer->amount_total_nis = $customer->amount_total_nis + $amount;
			$customer->update();

			return true;
		} else {
			return false;
		}
	}

	public static function withdraw_money ($id, $amount) {
		$customer = CustomerClient::find($id);
		if ($customer && $customer->amount_total_nis > $amount) {
			$customer->amount_total_nis = $customer->amount_total_nis - $amount;
			$customer->update();

			return true;
		} else {
			return false;
		}
	}

	public static function get_credit ($id) {
		$customer = CustomerClient::find($id);
		if ($customer) {
			return response()->json($customer->amount_total_nis);
		}

		return response()->json(Null);
	}
}
