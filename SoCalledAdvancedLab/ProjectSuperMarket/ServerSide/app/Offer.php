<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


/**
 * App\Offer
 *
 * @property-read \App\Product $product
 * @property-read \App\Store $store
 * @mixin \Eloquent
 */
class Offer extends Model {

	public function product () {
		return $this->belongsTo(Product::class);
	}

	public function store () {
		return $this->belongsTo(Store::class);
	}

}
