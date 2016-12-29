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
			$table->string('public_key')->unique();
			$table->string('current_session_key')->unique();
			$table->boolean('valid');

			// In case of legal uses
			$table->string('ID_num')->unique();
			$table->string('phone_number')->unique();

			// For easy access -> this keeps getting altered periodically
			// can be used to know the amount of money -> max that can be withdrawn
			// FIXME could be unnecessary
			$table->unsignedInteger('amount_total_nis');

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
