<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


class CustomerClient extends Model {
	/**
	 * Represent the normal users of the system
	 * Contains the credit owned by each user
	 * As well as other information regarding them
	 **/

	public function transactions () {
		return $this->hasMany(VehicleCustomerTransaction::class);
	}

	public function deposits () {
		return $this->hasMany(CustomerDeposit::class);
	}
}
