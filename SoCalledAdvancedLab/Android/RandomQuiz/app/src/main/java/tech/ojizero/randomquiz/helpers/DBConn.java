package tech.ojizero.randomquiz.helpers;

/**
 * Created by oji on 3/9/17.
 */

import android.content.Context;
import android.database.DatabaseErrorHandler;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DBConn extends SQLiteOpenHelper {
	public static String
		TableName = "products",
		DbName;

	private SQLiteDatabase
		db;


	public DBConn (
		Context context, String name,
		SQLiteDatabase.CursorFactory factory,
		int version
	) {
		super(context, name, factory, version);
		DbName = name;
	}

	public DBConn (
		Context context, String name, SQLiteDatabase.CursorFactory factory,
		int version,
		DatabaseErrorHandler errorHandler
	) {
		super(context, name, factory, version, errorHandler);
		DbName = name;
	}

	@Override
	public void onCreate (SQLiteDatabase sqLiteDatabase) {
		db.execSQL("" +
		           "CREATE TABLE IF NOT EXISTS" + TableName +
		           "(" +
		           "ID AUTO INCREMENT PRIMARY KEY," +
		           "NAME VARCHAR(50)," +
		           "PRICE INTEGER" +
		           ")" +
		           "");
	}

	@Override
	public void onUpgrade (SQLiteDatabase sqLiteDatabase, int i, int i1) {

	}


}
