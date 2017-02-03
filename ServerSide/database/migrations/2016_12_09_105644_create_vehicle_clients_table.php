<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateVehicleClientsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('vehicle_clients', function (Blueprint $table) {
//			$table->increments('id');

			// Credentials
			$table->string('id')->unique(); // had hu el permit
			$table->string('password');
			$table->boolean('valid');
//			$table->string('public_key')->unique()->nullable();

			// In case of legal uses
			$table->string('ID_num')->unique();
			$table->string('phone_number')->unique();

			$table->double('amount_total_nis')->default(0.0);

			$table->string('remember_token', 100)->nullable();
			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('vehicle_clients');
	}
}
