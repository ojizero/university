<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateVehicleWithdrawalsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		// ignore the name for now
		Schema::create('vehicle_withdrawals', function (Blueprint $table) {
			$table->increments('id');

			$table->integer('vehicle_client_id');
			$table->double('withdrawn_amount_nis');

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('vehicle_withdrawals');
	}
}
