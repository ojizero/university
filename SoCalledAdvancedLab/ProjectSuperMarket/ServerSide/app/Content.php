<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Content extends Model {

	protected $fillable = [
		'foreign_id', 'foreign_type', 'content_path', 'content_type', 'meta',
	];

	public function foreign () {
		return $this->belongsTo($this->foreign_type);
	}

	public function foreignType () {
		return $this->foreign_type;
	}

}
