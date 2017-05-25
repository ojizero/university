<?php namespace App\Http\Middleware;

use App\Models\User;
use App\Models\Role;
use Closure;
use Illuminate\Contracts\Auth\Guard;
use Illuminate\Database\Eloquent\Collection;

class CheckPermission {

	/**
	 * The Guard implementation.
	 *
	 * @var Guard
	 */
	protected $auth;

	/**
	 * Create a new filter instance.
	 *
	 * @param  Guard  $auth
	 * @return void
	 */
	public function __construct (Guard $auth) {
		$this->auth = $auth;
	}

	/**
	 * Handle an incoming request.
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \Closure  $next
	 * @return mixed
	 */
	public function handle ($request, Closure $next) {
		if ($this->auth->guest()) {
			$user = new User;
			$user->id = 0;
			$user->username = 'Guest';

			$permission = Role::find(8); # had el view_content permission

			$user->attachPermission($permission);
			// somehow override the current user with this new one ?
		} else {
			$user = $this->auth->user();
		}

		return $next($request);
	}

}