<?php

use App\Role;
use App\User;
use Illuminate\Database\Seeder;

class UsersTableSeeder extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run () {
		# Admin user
		User::create([
			'name'     => 'admin',
			'email'    => 'admin@mrkt.io',
			'password' => Hash::make('admin'),
		])->attachRole(Role::where('name', '=', '$admin')->first());

		# Owner user
		User::create([
			'name'     => 'Some Owner',
			'email'    => 'mrkt@mail.co',
			'password' => Hash::make('owner'),
		])->attachRole(Role::where('name', '=', '$owner')->first());

		# Customer user
		User::create([
			'name'     => 'user',
			'email'    => 'user@mail.xyz',
			'password' => Hash::make('user'),
		])->attachRole(Role::where('name', '=', '$user')->first());
	}
}
