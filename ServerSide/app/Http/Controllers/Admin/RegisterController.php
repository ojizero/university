<?php

namespace App\Http\Controllers\Admin;


use App\Admin;
use App\Http\Controllers\Controller;
use Auth;
use Illuminate\Contracts\Validation\Validator;
use Illuminate\Foundation\Auth\RegistersUsers;


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
	protected $redirectTo = '/admin_home';

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
			'password' => 'required|min:8|confirmed',
		]);
	}

	/**
	 * Create a new user instance after a valid registration.
	 *
	 * @param  array $data
	 * @return Admin
	 */
	protected function create (array $data) {
		return Admin::create([
			'password' => bcrypt($data['password']),
		]);
	}

	protected function guard () {
		return Auth::guard('admin');
	}
}
