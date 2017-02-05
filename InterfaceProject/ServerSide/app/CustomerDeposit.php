<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


class CustomerDeposit extends Model {
	/**
	 * Contains the payments made by the users to
	 * purchase credit for the cards/app
	 **/

	public function customer () {
		return $this->belongsTo(CustomerClient::class);
	}
}
