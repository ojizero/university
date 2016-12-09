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
			$table->increments('id');

			// Credentials
			$table->string('permit')->unique();
			$table->string('password');
			$table->string('public_key');
			$table->string('current_session_key');

			// In case of legal uses
			$table->string('ID_num');
			$table->string('phone_number');

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
