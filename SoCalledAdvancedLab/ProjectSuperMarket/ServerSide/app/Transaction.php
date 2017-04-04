<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


/**
 * App\Transaction
 *
 * @property-read \App\Customer $customer
 * @property-read \App\Product $product
 * @property-read \App\Store $store
 * @mixin \Eloquent
 */
class Transaction extends Model {

	public function customer () {
		return $this->belongsTo(Customer::class);
	}

	public function store () {
		return $this->belongsTo(Store::class);
	}

	public function product () {
		return $this->belongsTo(Product::class);
	}

}
