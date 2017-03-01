package tech.ojizero.layouts;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

public class AddActivity extends AppCompatActivity {
	@RequiresApi (api = Build.VERSION_CODES.N)
	@Override
	protected void onCreate (Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_add);

		Spinner listPicker = (Spinner) findViewById(R.id.GenderList);
		listPicker.setAdapter(
			new ArrayAdapter(this, R.layout.activity_add, Customer.Genders)
		);

		findViewById(R.id.AddButton).setOnClickListener(v -> {
			Customer customer =
				new Customer(
					Integer.parseInt(((EditText) findViewById(R.id.IdText)).getText().toString()),
					((EditText) findViewById(R.id.NameText)).getText().toString(),
					((EditText) findViewById(R.id.PhoneText)).getText().toString(),
					"", //listPicker.getSelectedItem().toString(),
					null
				);
			Customer.List.add(customer);
		});

		findViewById(R.id.BackButton).setOnClickListener(v -> {
			AddActivity.this.startActivity(
				new Intent(AddActivity.this, MainActivity.class)
			);
			finish();
		});
	}
}
