<?php

namespace App;


use Illuminate\Database\Eloquent\Model;


/**
 * App\Store
 *
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Offer[] $offers
 * @property-read \App\User $owner
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Product[] $products
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Transaction[] $transactions
 * @mixin \Eloquent
 */
class Store extends Model {

	public static $name = 'store';

	protected $fillable = [
		'user_id', 'store_name', 'store_logo',
	];

	public function products () {
		return $this->hasMany(Product::class);
	}

	public function offers () {
		return $this->hasMany(Offer::class);
	}

	public function owner () {
		return $this->belongsTo(User::class);
	}

	public function transactions () {
		return $this->hasMany(Transaction::class);
	}

	public function contents () {
		return $this->hasMany(Content::class, 'foreign_id');
	}

}
