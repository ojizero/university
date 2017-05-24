<?php

namespace App\Http\Middleware;

use App;
use Closure;
use \Illuminate\Http\Request;

use \App\Content;

class ContentMiddleware {
	protected $contentfulRoutes  = [
		'stores', 'products',
	];

	protected $contentfulClasses = [
		'stores'   => \App\Store::class,
		'products' => \App\Product::class,
	];

	/**
	 * Handle an incoming request. Not safe in real life,
	 * but I'm too lazy to bother finding a better way
	 *
	 * @param  \Illuminate\Http\Request  $request
	 * @param  \Closure  $next
	 * @return mixed
	 */
	public function handle (Request $request, Closure $next) {

		$contentful = $this->isContentful($request->route()->uri)[0];
		if (array_key_exists('content_object', $request->all())	&& $contentful) {
			if (False && ! \Entrust::can('manage_content')) {
				return response()->json([
					'status'   => 403,
					'response' => 'unauthorized access',
				], 403);
			}

			$contentClass = App::make($this->contentfulClasses[$contentful]);
			$classTable   = with($contentClass)->getTable();

			$foreign_id = \DB::select("show table status like '${classTable}'")[0]->Auto_increment;

			$request['_content_result'] = (new \App\Http\Controllers\ContentController())->createFor($request['content_object'], $foreign_id, $this->contentfulClasses[$contentful]);
			$request->request->remove('content_object');
			$request['id'] = $foreign_id;
		}

		return $next($request);
	}

	public function isContentful ($uri) {
		$matches = [];
		foreach ($this->contentfulRoutes as $route) {
			if (preg_match('('.$route.')', $uri, $matches)) {
				return $matches;
			}
		}
		return false;
	}
}
