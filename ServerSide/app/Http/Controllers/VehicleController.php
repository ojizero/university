<?php

namespace App\Http\Controllers;


use App\VehicleClient;


class VehicleController extends Controller {
	public static function deposit_money ($id, $amount) {
		$vehicle = VehicleClient::find($id);
		if ($vehicle) {
			$vehicle->amount_total_nis = $vehicle->amount_total_nis + $amount;
			$vehicle->update();

			return true;
		} else {
			return false;
		}
	}

	public function admin_deposit_money ($id, $amount) {

	}

	public static function withdraw_money ($id, $amount) {

	}

	public function admin_withdraw_money ($id, $amount) {

	}

	public static function get_credit ($id) {

	}

	public function admin_get_credit ($id, $amount) {

	}

	public static function validate_authorization ($id) {
		return true;
	}

	/*
	|--------------------------------------------------------------------------
	| Invalidate the vehicle due to malicious activity
	|--------------------------------------------------------------------------
	|
	| In response of any suspecion of malicious activity invalidate all credentials
	| and stop recognizing vehicle
	|
	*/
	public function invalidate ($id) {
		if (VehicleController::validate_authorization($id)) {
			$vehicle        = VehicleClient::find($id);
			$vehicle->valid = false;
			$vehicle->update();
		}
	}
}
