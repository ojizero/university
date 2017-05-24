<?php

use App\Role;
use App\Permission;
use Illuminate\Database\Seeder;

class EntrustTablesSeedeer extends Seeder
{
	/**
	 * Run the database seeds.
	 *
	 * @return void
	 */
	public function run () {
		/**
		 * User roles
		 */
		# Store owners
		$owner = Role::create([
			'name'         => '$owner',
			'display_name' => 'Store Owner Role',
			'description'  => 'role for store owners',
		]);

		# Admins
		$admin = Role::create([
			'name'         => '$admin',
			'display_name' => 'Administrator',
			'description'  => 'role for administrators',
		]);

		# Customers
		$user = Role::create([
			'name'         => '$user',
			'display_name' => 'Customer',
			'description'  => 'role for normal customers',
		]);

		/**
		 * Role permission
		 */
		# manage users
		$manageUsers = Permission::create([
			 'name'         => 'manage_users',
			 'display_name' => 'manage_users',
			 'description'  => 'manage_users',
		]);
		# guest signup
		$signUp = Permission::create([
			 'name'         => 'sign_up',
			 'display_name' => 'sign_up',
			 'description'  => 'sign_up',
		]);

		# manage content
		$manageContent = Permission::create([
			 'name'         => 'manage_content',
			 'display_name' => 'manage_content',
			 'description'  => 'manage_content',
		]);

		# view content
		$viewContent = Permission::create([
			 'name'         => 'view_content',
			 'display_name' => 'view_content',
			 'description'  => 'view_content',
		]);

		# make transactions
		$makeTransaction = Permission::create([
			 'name'         => 'make_transaction',
			 'display_name' => 'make_transaction',
			 'description'  => 'make_transaction',
		]);

		/**
		 * Attach permissions to roles
		 */
		$admin->attachPermissions([
			$manageUsers,
			$manageContent,
			$viewContent,
			$makeTransaction,
		]);

		$owner->attachPermissions([
			$manageContent,
			$viewContent,
			$makeTransaction,
		]);

		$user->attachPermissions([
			$viewContent,
			$makeTransaction,
		]);
	}
}
