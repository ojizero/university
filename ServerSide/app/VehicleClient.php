<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


class VehicleClient extends Model {
	/**
	 * Represents the drivers (taxis) of the system
	 * Contains information as well as credentials
	 **/

	public function transactions () {
		return $this->hasMany(VehicleCustomerTransaction::class);
	}

	public function withdrawals () {
		return $this->hasMany(VehicleWithdrawal::class);
	}
}
