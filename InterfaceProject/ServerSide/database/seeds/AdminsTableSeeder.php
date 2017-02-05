<?php

use Illuminate\Database\Seeder;
use App\Admin;

class AdminsTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run() {
    	// make an admin user .. id=1 (first admin) password=adminer
    	Admin::create([
    		'password' => Hash::make('adminer'),
    	]);
    }
}
