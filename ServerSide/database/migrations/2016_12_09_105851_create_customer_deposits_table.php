<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateCustomerDepositsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('customer_deposits', function (Blueprint $table) {
			$table->increments('id');

			$table->integer('customer_client_id');
			$table->unsignedInteger('deposited_amount_nis');

//			$table->integer('method_id'); // suggested

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('consumer_deposits');
	}
}
