<?php

namespace App\Http\Controllers;


use App\CustomerClient;
use App\VehicleClient;
use App\VehicleCustomerTransaction;


class TransactionsController extends Controller {
	/*
	|--------------------------------------------------------------------------
	| Create a new transaction
	|--------------------------------------------------------------------------
	|
	| New transactions to be valid require the amount transferred to be s-
	| -trictly positive, the customer and vehicle to exist and the cus-
	| -tomer to have credit equal to or greater than the amoutn transferred
	|
	*/
	public function transaction_request ($from, $to, $amount) {
		$customer = CustomerClient::find($to);
		if (($amount > 0) && $customer && ($customer->credit >= $amount) && VehicleClient::find($to)) {
			$transaction                     = new VehicleCustomerTransaction();
			$transaction->amount_nis         = $amount;
			$transaction->vehicle_client_id  = $to;
			$transaction->customer_client_id = $from;

			VehicleController::deposit_money($to, $amount);
			CustomerController::withdraw_money($from, $amount);

			return response()->json(True, 200);;
		} else {
			// return null on fail
			return response()->json(Null, 200);
		}
	}
}
