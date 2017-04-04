<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateTransactionsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('transactions', function (Blueprint $table) {
			$table->increments('id');

			$table->integer('user_id')->nullable(false);
			$table->integer('store_id')->nullable(false);
			$table->integer('product_id')->nullable(false);

			$table->unsignedInteger('amount')->default(0);

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('transactions');
	}
}
