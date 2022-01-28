package com.energy.accounting.socket;

import android.content.Context;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;

public class MainActivity extends AppCompatActivity {

    TextView textView2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void getLocation(View view) {
       // ActivityCompat.requestPermissions(this,new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);
        //LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        // get the last know location from your location manager.
        try {
            //Location location = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            downloadFile();
            //textView2.setText(String.format("%.3f", location.getLatitude())+" , "+String.format("%.3f",location.getLongitude()));
        }catch (Exception e){
            e.printStackTrace();
        }

    }

    public  void downloadFile() {


        new Thread(new Runnable(){
            @Override
            public void run() {
                try {
                    URL u = new URL("https://static.googleusercontent.com/media/research.google.com/en//archive/paxos_made_live.pdf");
                    URLConnection conn = u.openConnection();
                    conn.connect();
                    int contentLength = conn.getContentLength();

                    DataInputStream stream = new DataInputStream(u.openStream());
                    File file = new File(getFilesDir(), "new.pdf");

                    DataOutputStream fos = new DataOutputStream(openFileOutput("new_"+System.currentTimeMillis()+".pdf", Context.MODE_PRIVATE));

                    byte[] buffer = new byte[1024];
                    int len1 = 0;
                    while ((len1 = stream.read(buffer)) > 0) {
                        fos.write(buffer, 0, len1);
                    }
                    stream.close();

                    // this should be called when the download is completed else toast should be shown if there
                    // not enough credits
                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast.makeText(MainActivity.this, "Download completed", Toast.LENGTH_SHORT).show();
                        }
                    });

                } catch(FileNotFoundException e) {
                    return;
                } catch (IOException e) {
                    showToast();
                    return;
                }
            }
        }).start();

    }

    public void showToast(){
        runOnUiThread(new Runnable() {
            public void run() {
                CharSequence text = "App doesn't have enough credits";
                Toast.makeText(MainActivity.this, text, Toast.LENGTH_SHORT).show();
            }
        });
    }
}

