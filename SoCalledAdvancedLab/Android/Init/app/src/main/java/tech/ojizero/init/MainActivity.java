package tech.ojizero.init;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity implements View.OnClickListener {

	@Override
	protected void onCreate (Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		findViewById(R.id.Max).setOnClickListener(this);
		findViewById(R.id.Min).setOnClickListener(this);
		findViewById(R.id.Avg).setOnClickListener(this);
		findViewById(R.id.Add).setOnClickListener(this);
		findViewById(R.id.Mul).setOnClickListener(this);
	}

	@Override
	public void onClick (View view) {
		EditText result = (EditText) findViewById(R.id.EditText);
		Double   x, y, z;
		try {
			x = Double.parseDouble(((EditText) findViewById(R.id.editText1)).getText().toString());
			y = Double.parseDouble(((EditText) findViewById(R.id.editText2)).getText().toString());
			z = Double.parseDouble(((EditText) findViewById(R.id.editText3)).getText().toString());
		} catch (NumberFormatException ignored) {
			return;
		}

		switch (view.getId()) {
			case R.id.Max:
				result.setText(String.format("%s", Math.max(Math.max(x, y), z)));
				break;

			case R.id.Min:
				result.setText(String.format("%s", Math.min(Math.min(x, y), z)));
				break;

			case R.id.Avg:
				result.setText(String.format("%s", (x + y + z) / 3));
				break;

			case R.id.Add:
				result.setText(String.format("%s", x + y + z));
				break;

			case R.id.Mul:
				result.setText(String.format("%s", x * y * z));
				break;

			default:
				result.setText("IMPOSSIBLE !!");
		}
	}
}
