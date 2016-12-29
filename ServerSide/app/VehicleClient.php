<?php

namespace App;


use Illuminate\Foundation\Auth\User as Authenticatable;


class VehicleClient extends Authenticatable {
	/**
	 * The attributes that should be hidden for arrays.
	 *
	 * @var array
	 */
	protected $hidden = [
		'password', 'remember_token',
	];

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

	// protected function guard()
	// {
	// 	return Auth::guard('guard-name');
	// }
}
