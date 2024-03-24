package com.senai.petpetisco;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;
import java.util.List;

public class DBHelper extends SQLiteOpenHelper {
    private static final String DATABASE_NAME = "petpetisco.db";
    private static final int DATABASE_VERSION = 1;
    private static final String TABLE_NAME = "schedules";
    private static final String COLUMN_ID = "_id";
    private static final String COLUMN_NAME = "time";
    // Adicione outras colunas conforme necessário

    public DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String query = "CREATE TABLE " + TABLE_NAME + " (" +
                COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT, " +
                COLUMN_NAME + " VARCHAR(20))";
        // Adicione outras colunas à consulta conforme necessário
        db.execSQL(query);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);
        onCreate(db);
    }

    public void addTimes(String nome) {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(COLUMN_NAME, nome);
        db.insert(TABLE_NAME, null, values);
        db.close();
    }

    public List<String> getAllTimes() {
        List<String> times = new ArrayList<>();
        String query = "SELECT * FROM " + TABLE_NAME;
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor cursor = db.rawQuery(query, null);
        if (cursor.moveToFirst()) {
            do {
                String nome = cursor.getString(cursor.getColumnIndexOrThrow(COLUMN_NAME));
                times.add(nome);
            } while (cursor.moveToNext());
        }
        cursor.close();
        db.close();
        return times;
    }
}

