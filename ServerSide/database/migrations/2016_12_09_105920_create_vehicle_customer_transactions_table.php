<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateVehicleCustomerTransactionsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('vehicle_customer_transactions', function (Blueprint $table) {
			$table->increments('id');

			$table->integer('customer_client_id');
			$table->integer('vehicle_client_id');
			$table->unsignedInteger('amount_nis');

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('vehicle_consumer_transactions');
	}
}
