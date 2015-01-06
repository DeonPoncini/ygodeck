package com.example.ygodeck;

import android.content.Context;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import net.sectorsoftware.ygo.deck.DB;
import net.sectorsoftware.ygo.deck.User;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends ActionBarActivity {

    static {
        System.loadLibrary("ygodeck-jni");
    }

    private EditText mUserNameText;
    private User mUser;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        loadCardFile();

        Button userLogin = (Button) findViewById(R.id.button_user_login);
        userLogin.setOnClickListener(mOnUserLogin);

        Button userAdd = (Button) findViewById(R.id.button_user_add);
        userAdd.setOnClickListener(mOnUserAdd);

        Button userRemove = (Button) findViewById(R.id.button_user_remove);
        userRemove.setOnClickListener(mOnUserRemove);

        mUserNameText = (EditText) findViewById(R.id.text_user_name);

    }

    private View.OnClickListener mOnUserLogin = new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            String text = mUserNameText.getText().toString();
            if (text.isEmpty()) {
                Toast.makeText(MainActivity.this, "Please enter a username", Toast.LENGTH_SHORT).show();
            } else {
                mUser = new User(text);
            }
        }
    };

    private View.OnClickListener mOnUserAdd = new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            String text = mUserNameText.getText().toString();
            if (text.isEmpty()) {
                Toast.makeText(MainActivity.this, "Please enter a username", Toast.LENGTH_SHORT).show();
            } else {
                mUser = new User(text, true);
            }
        }
    };

    private View.OnClickListener mOnUserRemove = new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            String text = mUserNameText.getText().toString();
            if (text.isEmpty()) {
                Toast.makeText(MainActivity.this, "Please enter a username", Toast.LENGTH_SHORT).show();
            } else {
                mUser = new User(text);
                mUser.remove();
                mUser.delete();
            }
        }
    };

    private void loadCardFile()
    {
        // copy the card database into an accessible location
        String cardFile = "card.db";
        String filename = getFilesDir() + File.separator + cardFile;
        try {
            // check if the file exists first
            File f = new File(filename);
            if (!f.exists()) {
                // copy the card db file so it is accessible by native applications
                InputStream fis = getResources().openRawResource(R.raw.card);
                FileOutputStream fos = openFileOutput(cardFile, Context.MODE_PRIVATE);
                int bytesRead = 0;
                do {
                    byte[] data = new byte[1024];
                    bytesRead = fis.read(data);
                    if (bytesRead == -1 || bytesRead == 0) {
                        break;
                    }
                    fos.write(data);
                } while (true);
                Toast.makeText(this, "Written file " + filename, Toast.LENGTH_LONG).show();
            }
            // setup the database
            DB.setPath(filename);
        } catch (FileNotFoundException e) {
            Toast.makeText(this, "Can't open " + filename, Toast.LENGTH_LONG).show();
        } catch (IOException e) {
            Toast.makeText(this, "Error reading " + filename, Toast.LENGTH_LONG).show();
        }
    }

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
