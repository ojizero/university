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

Route::get('/hey', function () {
	return response()->json([
		'A' => 'B',
	], 200);
});

Auth::routes();

Route::get('/transfer/{from}/{to}/{amount}', [
	'uses' => 'TransactionsController@transaction_request', // this is the controller's name followed by the function within the controller
	'as'   => 'make_transaction'  // this is the name of the route, it can be used to be references by HTML pages anywhere
]);

/*
|--------------------------------------------------------------------------
| Kill switch route
|--------------------------------------------------------------------------
|
| Invalidates vehicle in case of malicious suspicion
|
*/
Route::get('/kill/{id}', [
	'uses' => 'VehicleController@invalidate',
	'as'   => 'invalidate_id'
]);

/*
|--------------------------------------------------------------------------
| Routes regarding direct access to vehicle info
|--------------------------------------------------------------------------
|
| Query credit, deposit dn withdraw credit
|
*/
Route::get('/query/vehicle/credit/{id}', [
	'uses' => 'VehicleController@admin_get_credit',
	'as'   => 'vehicle_get_credit'
]);
Route::get('/query/vehicle/withdraw/{id}/{amount}', [
	'uses' => 'VehicleController@admin_withdraw_money',
	'as'   => 'vehicle_withdraw_credit'
]);
Route::get('/query/vehicle/deposit/{id}/{amount}', [
	'uses' => 'VehicleController@admin_deposit_money',
	'as'   => 'vehicle_deposit_credit'
]);

/*
|--------------------------------------------------------------------------
| Routes regarding direct access to customer info
|--------------------------------------------------------------------------
|
| Query credit, deposit dn withdraw credit
|
*/
Route::get('/query/customer/credit/{id}', [
	'uses' => 'CustomerController@admin_get_credit',
	'as'   => 'customer_get_credit'
]);
Route::get('/query/customer/withdraw/{id}/{amount}', [
	'uses' => 'CustomerController@admin_withdraw_money',
	'as'   => 'customer_withdraw_credit'
]);
Route::get('/query/customer/deposit/{id}/{amount}', [
	'uses' => 'CustomerController@admin_deposit_money',
	'as'   => 'customer_deposit_credit'
]);
