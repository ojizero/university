<?php

namespace App\Http\Controllers\Auth;


use App\Http\Controllers\Controller;
use App\VehicleClient;
use Illuminate\Foundation\Auth\RegistersUsers;
use Illuminate\Support\Facades\Validator;


class RegisterController extends Controller {
	/*
	|--------------------------------------------------------------------------
	| Register Controller
	|--------------------------------------------------------------------------
	|
	| This controller handles the registration of new users as well as their
	| validation and creation. By default this controller uses a trait to
	| provide this functionality without requiring any additional code.
	|
	*/

	use RegistersUsers;

	/**
	 * Where to redirect users after login / registration.
	 *
	 * @var string
	 */
	protected $redirectTo = '/home';

	/**
	 * Create a new controller instance.
	 *
	 * @return void
	 */
	public function __construct () {
		$this->middleware('guest');
	}

	/**
	 * Get a validator for an incoming registration request.
	 *
	 * @param  array $data
	 * @return \Illuminate\Contracts\Validation\Validator
	 */
	protected function validator (array $data) {
		return Validator::make($data, [
			'permit'       => 'unique:vehicle_clients', // TODO validate with max and min as the allowed length for permits
			'ID_num'       => 'unique:vehicle_clients', // TODO validate to only accept ID numbers
			'phone_number' => 'unique:vehicle_clients', // TODO validate to only accept valid phone numbers
			'password'     => 'required|min:8|confirmed',
		]);
	}

	/**
	 * Create a new user instance after a valid registration.
	 *
	 * @param  array $data
	 * @return VehicleClient
	 */
	protected function create (array $data) {
		return VehicleClient::create([
			'permit'       => $data['permit'],
			'ID_num'       => $data['idnum'],
			'phone_number' => $data['phone'],
			'password'     => bcrypt($data['password']),
			'valid'        => true,
		]);
	}
}
