package com.example.ygodeck;

import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Spinner;

import net.sectorsoftware.ygo.deck.DeckSet;
import net.sectorsoftware.ygo.deck.Format;
import net.sectorsoftware.ygo.deck.User;

import java.util.List;


public class DeckSetActivity extends ActionBarActivity {

    private User mUser;
    private ArrayAdapter<String> mListAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_deck_set);

        Intent intent = getIntent();
        String username = intent.getStringExtra(MainActivity.EXTRA_USER);
        mUser = new User(username);
        List<DeckSet> deckSets = mUser.deckSets();
        ListView listView = (ListView) findViewById(R.id.listView_deck_sets);
        mListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        listView.setAdapter(mListAdapter);

        for (DeckSet d : deckSets) {
            mListAdapter.add(d.name());
        }
        mListAdapter.notifyDataSetChanged();

        Spinner spinnerFormat = (Spinner) findViewById(R.id.spinner_format);
        ArrayAdapter<String> spinnerFormatAdapter = new ArrayAdapter<String>(DeckSetActivity.this,
                android.R.layout.simple_spinner_dropdown_item, Format.formats());
        spinnerFormat.setAdapter(spinnerFormatAdapter);

        Spinner spinnerFormatDate = (Spinner) findViewById(R.id.spinner_format_date);
        ArrayAdapter<String> spinnerFormatDateAdapter = new ArrayAdapter<String>(
                DeckSetActivity.this, android.R.layout.simple_spinner_dropdown_item,
                Format.formatDates());
        spinnerFormatDate.setAdapter(spinnerFormatDateAdapter);

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_deck_set, menu);
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
