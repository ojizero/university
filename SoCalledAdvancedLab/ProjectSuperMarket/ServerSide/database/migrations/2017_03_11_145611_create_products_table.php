<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class CreateProductsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('products', function (Blueprint $table) {
			$table->increments('id');

			// $table->integer('store_id');

			$table->string('product_name')->nullable(false);
			$table->string('product_description')->nullable(true);
			$table->unsignedInteger('product_price')->nullable(false);
			$table->boolean('availability')->nullable(false);

			# comma separated paths to files
			$table->string('images_paths')->nullable(true);

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('products');
	}
}
