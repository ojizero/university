<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


/**
 * App\Product
 *
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Offer[] $offers
 * @property-read \App\Store $store
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Transaction[] $transactions
 * @mixin \Eloquent
 */
class Product extends Model {

	public function store () {
		return $this->belongsTo(Store::class);
	}

	public function offers () {
		return $this->hasMany(Offer::class);
	}

	public function transactions () {
		return $this->hasMany(Transaction::class);
	}

}
