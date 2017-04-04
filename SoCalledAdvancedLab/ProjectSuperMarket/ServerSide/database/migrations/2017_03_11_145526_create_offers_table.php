<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateOffersTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('offers', function (Blueprint $table) {
			$table->increments('id');

			$table->integer('store_id');
			$table->integer('product_id');

			$table->unsignedTinyInteger('percent_discount')->nullable(false);
			$table->dateTimeTz('offer_begins')->nullable(false);
			$table->dateTimeTz('offer_ends')->nullable(true);

			$table->string('conditions')->nullable(true);
			$table->string('region')->nullable(true);

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('offers');
	}
}
