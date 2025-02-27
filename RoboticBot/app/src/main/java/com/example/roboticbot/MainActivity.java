package com.example.roboticbot;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {
    TextView mainread;
    FirebaseDatabase db = FirebaseDatabase.getInstance();
    DatabaseReference command, node;
    Button forward, back, left, right, stop;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainread = (TextView) findViewById(R.id.tagline);
        forward = (Button) findViewById(R.id.fwd);
        back = (Button) findViewById(R.id.back);
        left = (Button) findViewById(R.id.left);
        right = (Button) findViewById(R.id.right);
        stop = (Button) findViewById(R.id.stop);

        node = db.getReference("Setting");
        command = node.child("command");

        forward.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                command.setValue("FWD");
            }
        });
        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                command.setValue("BACK");
            }
        });
        left.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                command.setValue("LEFT");
            }
        });
        right.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                command.setValue("RIGHT");
            }
        });
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                command.setValue("STOP");
            }
        });

        FirebaseDatabase.getInstance().getReference(".info/connected").addValueEventListener(new ValueEventListener() {
            public void onDataChange(DataSnapshot snapshot) {
                if (((Boolean) snapshot.getValue(Boolean.class)).booleanValue()) {
                    MainActivity.this.mainread.setText("INTERNET CONNECTED ");
                } else {
                    MainActivity.this.mainread.setText("INTERNET CONNECTION FAILED");
                }
            }
            public void onCancelled(DatabaseError error) {
            }
        });
    }

    public void onBackPressed() {
        super.onBackPressed();
        finish();
    }
}