<?php

namespace App\Http\Controllers;


use App\VehicleClient;
use Illuminate\Support\Facades\Auth;


class VehicleController extends Controller {
	public static function deposit_money ($id, $amount) {
		$vehicle = VehicleClient::find($id);
		if ($vehicle) {
			$vehicle->amount_total_nis = $vehicle->amount_total_nis + $amount;
			$vehicle->update();

//			VehicleWithdrawal::create([
//
//			]);

			return response()->json(True);
		}

		return response()->json(False);
	}

	public static function withdraw_money ($id, $amount) {
		$vehicle = VehicleClient::find($id);
		if ($vehicle && $vehicle->amount_total_nis > $amount) {
			$vehicle->amount_total_nis = $vehicle->amount_total_nis - $amount;
			$vehicle->update();

			return response()->json(True);
		}

		return response()->json(False);
	}

	public static function get_credit ($id) {
		$vehicle = VehicleClient::find($id);
		if ($vehicle) {
			return response()->json($vehicle->amount_total_nis);
		}

		return response()->json(Null);
	}

	public static function validate_authorization ($id) {
		return (Auth::guard('web')->user()->id == $id) && VehicleClient::find($id)->valid;
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
	public function invalidate () {
		$logged_in = Auth::guard('web')->user();
		if ($logged_in) {
			$vehicle        = VehicleClient::find($logged_in->id);
			$vehicle->valid = false;
			$vehicle->update();

			return response()->json('dead');
		}

		return response()->json('alive');
	}
}
