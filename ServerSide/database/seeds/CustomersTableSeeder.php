<?php

use App\CustomerClient;
use Illuminate\Database\Seeder;


class CustomersTableSeeder extends Seeder {
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run () {
		CustomerClient::create([
			'secret_key'    => Hash::make('weam'),
			'ID_num'        => '651423',
			'phone_number'  => '56203',
			'customer_name' => 'Weam Mikkawi'
		]);

		CustomerClient::create([
			'secret_key'    => Hash::make('majdi'),
			'ID_num'        => '535533',
			'phone_number'  => '455645',
			'customer_name' => 'Majdi Lada'
		]);
	}
}
