package tech.ojizero.layouts;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;

public class MainActivity extends AppCompatActivity {

	@Override
	protected void onCreate (Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		setContentView(R.layout.activity_main);

		Button       btn   = new Button(this);
		ScrollView   view  = new ScrollView(this);
		LinearLayout outer = new LinearLayout(this);
		LinearLayout inner = new LinearLayout(this);

		outer.setRotation(LinearLayout.VERTICAL);
		inner.setRotation(LinearLayout.VERTICAL);

		btn.setText("Button");
		btn.setLayoutParams(new LinearLayout.LayoutParams(
			ActionBar.LayoutParams.WRAP_CONTENT,
			ActionBar.LayoutParams.WRAP_CONTENT
		));
		outer.addView(btn);
		outer.addView(view);
		view.addView(inner);
		setContentView(outer);

		btn.setOnClickListener(v -> {
			MainActivity.this.startActivity(new Intent(MainActivity.this, AddActivity.class));
			finish();
		});
	}
}
