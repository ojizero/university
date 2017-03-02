package tech.ojizero.dialer;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

	private final       String tickerText         = "Notification Message";
	private final       String contentTiltle      = "Notification";
	private final       String contentText        = "you have been notified !";
	public static final int    MY_NOTIFICATION_ID = 0;

	@Override
	protected void onCreate (Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		findViewById(R.id.Dialer).setOnClickListener(v -> {
			Intent newIntent = new Intent();

			newIntent.setAction(Intent.ACTION_DIAL);
			newIntent.setData(Uri.parse("tel:+150000"));

			startActivity(newIntent);
		});

		findViewById(R.id.Map).setOnClickListener(v -> {
			Intent myIntent = new Intent();

			myIntent.setAction(Intent.ACTION_VIEW);
			myIntent.setData(Uri.parse("geo:19.076,72.8777"));

			startActivity(myIntent);
		});

		findViewById(R.id.Mail).setOnClickListener(v -> {
			Intent intent = new Intent();

			intent.setAction(Intent.ACTION_SENDTO);
			intent.setType("message/rfc822");
			intent.setData(Uri.parse("mailto:"));
			intent.putExtra(Intent.EXTRA_EMAIL, "mhmdateeq@yahoo.com");
			intent.putExtra(Intent.EXTRA_SUBJECT, "my subject");
			intent.putExtra(Intent.EXTRA_TEXT, "content of the message");

			startActivity(intent);
		});

		findViewById(R.id.Notification).setOnClickListener(v -> {
			Notification.Builder builder = new Notification.Builder(getApplicationContext());
			builder.setTicker(tickerText);
			builder.setAutoCancel(true);
			builder.setContentTitle(contentTiltle);
			builder.setContentText(contentText);

//			builder.setContentIntent(mContentIntent);
//			builder.setSmallIcon(R.drawable.images);

			((NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE))
				.notify(MY_NOTIFICATION_ID, builder.build());

		});
	}
}
