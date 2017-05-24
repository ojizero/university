<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateContentsTable extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::create('contents', function (Blueprint $table) {
			$table->increments('id');

			$table->integer('foreign_id')->nullable(false);
			$table->string('foreign_type')->nullable(false);

			$table->string('content_path')->nullable(false);
			$table->string('content_type')->nullable(true);

			$table->string('meta')->nullable(true);

			$table->timestamps();
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::dropIfExists('contents');
	}
}
