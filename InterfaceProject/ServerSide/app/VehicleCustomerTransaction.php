<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


class VehicleCustomerTransaction extends Model {
	/**
	 * Payments made through the system
	 * daf3 el ajarat
	 **/

	public function vehicles () {
		return $this->belongsTo(VehicleClient::class);
	}

	public function customer () {
		return $this->belongsTo(CustomerClient::class);
	}
}
