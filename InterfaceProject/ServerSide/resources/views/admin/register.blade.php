@extends('layouts.app')

@section('content')
	<div class="container">
		<div class="row">
			<div class="col-md-8 col-md-offset-2">
				<div class="panel panel-default">
					<div class="panel-heading">Register</div>
					<div class="panel-body">
						<form class="form-horizontal" role="form" method="POST" action="{{ url('/register') }}">
							{{ csrf_field() }}

							<div class="form-group{{ $errors->has('permit') ? ' has-error' : '' }}">
								<label for="email" class="col-md-4 control-label">Permit</label>

								<div class="col-md-6">
									<input id="permit" type="permit" class="form-control" name="permit"
									       value="{{ old('permit') }}" required>

									@if ($errors->has('permit'))
										<span class="help-block">
                                        <strong>{{ $errors->first('permit') }}</strong>
                                    </span>
									@endif
								</div>
							</div>

							<div class="form-group{{ $errors->has('idnum') ? ' has-error' : '' }}">
								<label for="idnum" class="col-md-4 control-label">ID Number</label>

								<div class="col-md-6">
									<input id="idnum" type="idnum" class="form-control" name="idnum"
									       value="{{ old('idnum') }}" required>

									@if ($errors->has('idnum'))
										<span class="help-block">
                                        <strong>{{ $errors->first('idnum') }}</strong>
                                    </span>
									@endif
								</div>
							</div>

							<div class="form-group{{ $errors->has('phone') ? ' has-error' : '' }}">
								<label for="phone" class="col-md-4 control-label">Phone Number</label>

								<div class="col-md-6">
									<input id="phone" type="phone" class="form-control" name="phone"
									       value="{{ old('phone') }}" required>

									@if ($errors->has('phone'))
										<span class="help-block">
                                        <strong>{{ $errors->first('phone') }}</strong>
                                    </span>
									@endif
								</div>
							</div>

							<div class="form-group{{ $errors->has('password') ? ' has-error' : '' }}">
								<label for="password" class="col-md-4 control-label">Password</label>

								<div class="col-md-6">
									<input id="password" type="password" class="form-control" name="password" required>

									@if ($errors->has('password'))
										<span class="help-block">
                                        <strong>{{ $errors->first('password') }}</strong>
                                    </span>
									@endif
								</div>
							</div>

							<div class="form-group">
								<label for="password-confirm" class="col-md-4 control-label">Confirm Password</label>

								<div class="col-md-6">
									<input id="password-confirm" type="password" class="form-control"
									       name="password_confirmation" required>
								</div>
							</div>

							<div class="form-group">
								<div class="col-md-6 col-md-offset-4">
									<button type="submit" class="btn btn-primary">
										Register
									</button>
								</div>
							</div>
						</form>
					</div>
				</div>
			</div>
		</div>
	</div>
@endsection
