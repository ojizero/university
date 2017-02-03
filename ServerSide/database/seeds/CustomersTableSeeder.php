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
			'id'          => '022AB2C5',
			'ID_num'        => '651423',
			'phone_number'  => '56203',
			'customer_name' => 'Weam Mikkawi'
		]);

		CustomerClient::create([
			'id'          => '321A4B2B',
			'ID_num'        => '535533',
			'phone_number'  => '455645',
			'customer_name' => 'Majdi Lada'
		]);
	}
}
