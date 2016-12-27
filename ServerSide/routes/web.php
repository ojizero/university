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

Route::get('/transfer/{from}/{to}/{amount}', [
	'uses' => 'VehicleController@transaction_request', // this is the controller's name followed by the function within the controller
	'as'   => 'make_transaction'  // this is the name of the route, it can be used to be references by HTML pages anywhere
]);