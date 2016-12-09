<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateCustomerClientsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('customer_clients', function (Blueprint $table) {
			$table->increments('id');

			$table->string('secret_key')->unique();
			$table->unsignedInteger('credit')->default(0);

			$table->string('customer_name');
			$table->string('phone_number')->unique();
			$table->string('ID_num')->unique();

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('consumer_clients');
	}
}
