<?php

namespace App\Http\Controllers;


use App\CustomerClient;


class CustomerController extends Controller {
	public static function deposit_money ($id, $amount) {

	}

	public static function withdraw_money ($id, $amount) {
		$customer = CustomerClient::find($id);
		if ($id) {
			$customer->credit = $customer->credit - $amount;
			$customer->update();

			return true;
		} else {
			return false;
		}
	}

	public static function get_credit ($id) {

	}

	public function payment_authentication () {

	}
}
