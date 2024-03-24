package com.senai.petpetisco;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button btnAgenda = (Button) findViewById(R.id.btn_agendar);
        btnAgenda.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, scheduleActivity.class);
                startActivity(intent);
            }
        });





        Button btnLiberar = (Button) findViewById(R.id.btn_liberar);
        btnLiberar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MQQTComunication.sendMessage("sinal");
                Toast.makeText(MainActivity.this, "Compartimento liberado", Toast.LENGTH_SHORT).show();
            }
        });

    }
}