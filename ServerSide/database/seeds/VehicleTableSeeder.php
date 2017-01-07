<?php

use App\VehicleClient;
use Illuminate\Database\Seeder;


class VehicleTableSeeder extends Seeder {
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run () {
		VehicleClient::create([
			'permit'       => '123456',
			'password'     => '12345678',
			'valid'        => true,
			'ID_num'       => '1532',
			'phone_number' => '56256'
		]);
	}
}
