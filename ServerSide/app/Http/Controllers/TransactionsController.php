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
	| New transactions to be valid require the amount transferred to be strictly
	| positive, the customer and vehicle to exist and the customer to have
	| credit equal to or greater than the amoutn transferred
	|
	*/
	public function transaction_request ($from, $to, $amount) {
		if (VehicleController::validate_authorization($to)) { // if authorized
			// get customer with ID $from
			$customer = CustomerClient::find($from);
			$vehicle  = VehicleClient::find($to);
			// do the validation part
			// customer exists, amount is positive, customer has credit, vehicle exists
			if ((($amount > 0) && $customer && ($customer->credit >= $amount)) && $vehicle && $vehicle->valid) {
				// create new transaction object
				$transaction                     = new VehicleCustomerTransaction();
				$transaction->amount_nis         = $amount;
				$transaction->vehicle_client_id  = $to;
				$transaction->customer_client_id = $from;

				// apply the transaction between customer and vehicle
				VehicleController::deposit_money($to, $amount);
				CustomerController::withdraw_money($from, $amount);
				// save the transaction
				$transaction->save();

				return response()->json(True, 200);
			} else {
				// return null on fail
				return response()->json(Null, 200);
			}
		} else {

		}
	}
}
