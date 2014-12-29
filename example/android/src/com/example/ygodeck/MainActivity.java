package com.example.ygodeck;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import net.sectorsoftware.ygodeck.Example;

public class MainActivity extends ActionBarActivity {

    static {
        System.loadLibrary("ygodeck-jni");
    }

    private Example mExample = new Example();
    private int mCounter = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button increment = (Button) findViewById(R.id.button);
        increment.setOnClickListener(mOnIncrement);
    }

    private View.OnClickListener mOnIncrement = new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            mCounter = mExample.increment(mCounter);
            Toast.makeText(MainActivity.this, Integer.toString(mCounter), Toast.LENGTH_SHORT).show();
        }
    };

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
}
