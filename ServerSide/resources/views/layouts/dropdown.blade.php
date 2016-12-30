<li class="dropdown">
	<a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">
		{{--{{ Auth::user()->name }}--}}
		@if(!Auth::guard())
			An Admin!
		@else
			A User.
		@endif
		<span class="caret"></span>
	</a>

	<ul class="dropdown-menu" role="menu">
		<li>
			<a href="
			@if(!Auth::guard())
			{{ url('/admin_logout') }}
			@else
			{{ url('/logout') }}
			@endif
					"
			   onclick="event.preventDefault();
                                                     document.getElementById('logout-form').submit();">
				Logout
			</a>

			<form id="logout-form" action="{{ url('/logout') }}" method="POST" style="display: none;">
				{{ csrf_field() }}
			</form>
		</li>
	</ul>
</li>