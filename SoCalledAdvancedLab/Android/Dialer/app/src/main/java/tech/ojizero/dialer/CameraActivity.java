package tech.ojizero.dialer;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.widget.ImageView;

public class CameraActivity extends AppCompatActivity {
	public static final int REQUESTED_ID = 1;

	@Override
	protected void onCreate (Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera);

		findViewById(R.id.GotoCamera).setOnClickListener(v -> {
			Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
			if (cameraIntent.resolveActivity(getPackageManager()) != null) {
				startActivityForResult(cameraIntent, REQUESTED_ID);
			}
		});

		findViewById(R.id.ImageView).setOnClickListener(v -> {
			startActivity(new Intent(CameraActivity.this, MainActivity.class));
			finish();
		});
	}

	@Override
	protected void onActivityResult (int requestCode, int resultCode, Intent data) {
		if (requestCode == REQUESTED_ID && resultCode == RESULT_OK) {
			((ImageView) findViewById(R.id.ImageView)).setImageBitmap(
				(Bitmap) data.getExtras().get("data")
			);
		} else {
			super.onActivityResult(requestCode, resultCode, data);
		}
	}
}
