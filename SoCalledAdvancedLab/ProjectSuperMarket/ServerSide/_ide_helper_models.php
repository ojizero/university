<?php
/**
 * A helper file for your Eloquent Models
 * Copy the phpDocs from this file to the correct Model,
 * And remove them from this file, to prevent double declarations.
 *
 * @author Barry vd. Heuvel <barryvdh@gmail.com>
 */


namespace App{
/**
 * App\Admin
 *
 * @mixin \Eloquent
 */
	class Admin extends \Eloquent {}
}

namespace App{
/**
 * App\Offer
 *
 * @property-read \App\Product $product
 * @property-read \App\Store $store
 * @mixin \Eloquent
 */
	class Offer extends \Eloquent {}
}

namespace App{
/**
 * App\Product
 *
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Offer[] $offers
 * @property-read \App\Store $store
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Transaction[] $transactions
 * @mixin \Eloquent
 */
	class Product extends \Eloquent {}
}

namespace App{
/**
 * App\Store
 *
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Offer[] $offers
 * @property-read \App\User $owner
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Product[] $products
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Transaction[] $transactions
 * @mixin \Eloquent
 */
	class Store extends \Eloquent {}
}

namespace App{
/**
 * App\Transaction
 *
 * @property-read \App\User $ser
 * @property-read \App\Product $product
 * @property-read \App\Store $store
 * @mixin \Eloquent
 * @property-read \App\User $customer
 */
	class Transaction extends \Eloquent {}
}

namespace App{
/**
 * App\User
 *
 * @property-read \Illuminate\Notifications\DatabaseNotificationCollection|\Illuminate\Notifications\DatabaseNotification[] $notifications
 * @mixin \Eloquent
 * @property-read \Illuminate\Database\Eloquent\Collection|\App\Transaction[] $transactions
 */
	class User extends \Eloquent {}
}

