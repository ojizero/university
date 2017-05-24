<?php

use Illuminate\Http\Request;


/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

Route::middleware('auth:api')->get('/user', function (Request $request) {
	return $request->user();
});

Route::resource('products', 'ProductController', [
	'except' => [
		'create', 'edit',
	]
]);

Route::resource('transactions', 'TransactionController', [
	'except' => [
		'create', 'edit', 'update',
	]
]);

Route::resource('contents', 'ContentController', [
	'except' => [
		'create', 'edit', 'update',
	]
]);

Route::resource('stores', 'StoreController', [
	'except' => [
		'create', 'edit', 'update',
	]
]);

Route::get('test', function (Request $request) {
	dd(\Entrust::user()->id);
});
