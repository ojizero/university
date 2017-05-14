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

	protected $fillable = [
		'product_name', 'product_description', 'product_price', 'availability', 'images_paths',
	];

	public function store () {
		return $this->belongsToMany(Store::class);
	}

	public function offers () {
		return $this->hasMany(Offer::class);
	}

	public function transactions () {
		return $this->hasMany(Transaction::class);
	}

}
