<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;


class ModifyVehicleClients extends Migration {
	/**
	 * Run the migrations.
	 *
	 * @return void
	 */
	public function up () {
		Schema::table('vehicle_clients', function (Blueprint $table) {
			$table->boolean('maintenance')->nullable()->default(false);
		});
	}

	/**
	 * Reverse the migrations.
	 *
	 * @return void
	 */
	public function down () {
		Schema::table('vehicle_clients', function (Blueprint $table) {
			$table->dropColumn('maintenance');
		});
	}
}
