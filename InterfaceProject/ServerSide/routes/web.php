<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| This file is where you may define all of the routes that are handled
| by your application. Just tell Laravel the URIs it should respond
| to using a Closure or controller method. Build something great!
|
*/

Route::get('/', function () {
	return view('welcome');
});

Route::get('/login-hey', function () {
	return response()->json([
		'User' => 'Logged',
	], 200);
})->middleware('auth:web');

Route::get('/admin-hey', function () {
	return response()->json([
		'User' => 'Admin !',
	], 200);
})->middleware('auth:admin');

/*
|--------------------------------------------------------------------------
| Regarding transactions
|--------------------------------------------------------------------------
|
*/
Route::get('/transactions/index', 'TransactionsController@index')->middleware('auth:admin');

Route::get('/transfer/{from}/{to}/{amount}', [
	'uses' => 'TransactionsController@transaction_request', // this is the controller's name followed by the function within the controller
	'as'   => 'make_transaction'  // this is the name of the route, it can be used to be references by HTML pages anywhere
])->middleware('auth:web');

//Route::get('/yo', 'VehicleController@validate_authorization');

/*
|--------------------------------------------------------------------------
| Kill switch route
|--------------------------------------------------------------------------
|
| Invalidates vehicle in case of malicious suspicion
|
*/
Route::get('/kill', [
	'uses' => 'VehicleController@invalidate',
	'as'   => 'invalidate_id'
])->middleware('auth:web');


Route::get('/maintenance', 'VehicleController@maintenance_status')->middleware('auth:web');

/*
|--------------------------------------------------------------------------
| Routes regarding direct access to vehicle info
|--------------------------------------------------------------------------
|
| Query credit, deposit dn withdraw credit
|
*/
Route::get('/query/vehicle/credit/{id}', [
	'uses' => 'VehicleController@get_credit',
	'as'   => 'vehicle_get_credit'
])->middleware('auth:admin');
Route::get('/query/vehicle/withdraw/{id}/{amount}', [
	'uses' => 'VehicleController@withdraw_money',
	'as'   => 'vehicle_withdraw_credit'
])->middleware('auth:admin');
Route::get('/query/vehicle/deposit/{id}/{amount}', [
	'uses' => 'VehicleController@deposit_money',
	'as'   => 'vehicle_deposit_credit'
])->middleware('auth:admin');

/*
|--------------------------------------------------------------------------
| Routes regarding direct access to customer info
|--------------------------------------------------------------------------
|
| Query credit, deposit dn withdraw credit
|
*/
Route::get('/query/customer/credit/{id}', [
	'uses' => 'CustomerController@get_credit',
	'as'   => 'customer_get_credit'
])->middleware('auth:admin');
Route::get('/query/customer/withdraw/{id}/{amount}', [
	'uses' => 'CustomerController@withdraw_money',
	'as'   => 'customer_withdraw_credit'
])->middleware('auth:admin');
Route::get('/query/customer/deposit/{id}/{amount}', [
	'uses' => 'CustomerController@deposit_money',
	'as'   => 'customer_deposit_credit'
])->middleware('auth:admin');


//--------------------------------------
//--------------------------------------
// temporary and probably to be removed
//--------------------------------------

// Scaffolding made by `php artisan make:auth`
Auth::routes();
Route::get('/home', 'HomeController@index'); ## the default guard of the auth middleware
Route::get('/logout', 'Auth\LoginController@get_logout');

// Scaffolding made by me
Route::get('admin_login', 'Admin\LoginController@showLoginForm');
Route::post('admin_login', 'Admin\LoginController@login');
Route::post('admin_logout', 'Admin\LoginController@logout');
//Route::post('admin_password/email', 'Admin\ForgotPasswordController@sendResetLinkEmail');
//Route::post('admin_password/reset', 'Admin\ResetPasswordController@reset');
//Route::get('admin_password/reset', 'Admin\ForgotPasswordController@showLinkRequestForm');
//Route::get('admin_password/reset/{token}', 'Admin\ResetPasswordController@showResetForm');
//Route::get('admin_register', 'Admin\RegisterController@showRegistrationForm')->middleware('auth:admin');
//Route::post('admin_register', 'Admin\RegisterController@register')->middleware('auth:admin');

Route::get('/admin_home', 'AdminHomeController@index');


Route::get('/test/{uid}', function ($uid) {
	return response()->json(\App\CustomerClient::findOrFail($uid));
});