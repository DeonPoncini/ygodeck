package com.example.ygodeck;

import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.Toast;

import net.sectorsoftware.ygo.data.DataTypes;
import net.sectorsoftware.ygo.deck.DeckSet;
import net.sectorsoftware.ygo.deck.Format;
import net.sectorsoftware.ygo.deck.User;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DeckSetActivity extends ActionBarActivity {

    private User mUser;
    private ListView mListView;
    private ArrayAdapter<String> mListAdapter;
    private EditText mDeckSetName;
    private Spinner mFormatSpinner;
    private Spinner mFormatDateSpinner;
    private Map<String, DeckSet> mDeckSets = new HashMap<String, DeckSet>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_deck_set);

        Intent intent = getIntent();
        String username = intent.getStringExtra(MainActivity.EXTRA_USER);
        mUser = new User(username);
        List<DeckSet> deckSets = mUser.deckSets();
        mListView = (ListView) findViewById(R.id.listView_deck_sets);
        mListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        mListView.setAdapter(mListAdapter);
        mListView.setOnItemClickListener(mOpenListener);
        mListView.setOnItemLongClickListener(mRemoveListener);

        for (DeckSet d : deckSets) {
            mListAdapter.add(d.name());
            mDeckSets.put(d.name(), d);
        }
        mListAdapter.notifyDataSetChanged();

        mFormatSpinner = (Spinner) findViewById(R.id.spinner_format);
        ArrayAdapter<String> spinnerFormatAdapter = new ArrayAdapter<String>(DeckSetActivity.this,
                android.R.layout.simple_spinner_dropdown_item, Format.formats());
        mFormatSpinner.setAdapter(spinnerFormatAdapter);

        mFormatDateSpinner = (Spinner) findViewById(R.id.spinner_format_date);
        ArrayAdapter<String> spinnerFormatDateAdapter = new ArrayAdapter<String>(
                DeckSetActivity.this, android.R.layout.simple_spinner_dropdown_item,
                Format.formatDates());
        mFormatDateSpinner.setAdapter(spinnerFormatDateAdapter);

        mDeckSetName = (EditText) findViewById(R.id.editText_deck_set_name);
        Button addDeckSetButton = (Button) findViewById(R.id.button_add_deck_set);
        addDeckSetButton.setOnClickListener(mAddListener);
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

    private View.OnClickListener mAddListener = new View.OnClickListener () {

        @Override
        public void onClick(View v) {
            String name = mDeckSetName.getText().toString().trim();
            if (name.isEmpty()) {
                Toast.makeText(DeckSetActivity.this, "Please enter a valid deck set name",
                        Toast.LENGTH_SHORT).show();
            } else {
                // get the format type
                String formatStr = mFormatSpinner.getSelectedItem().toString();
                DataTypes.Format format = DataTypes.Format.ADVANCED;

                if (formatStr.equals("Traditional")) {
                    format = DataTypes.Format.TRADITIONAL;
                }
                String formatDate = mFormatDateSpinner.getSelectedItem().toString();
                Format f = new Format(format, formatDate);
                DeckSet deckSet = new DeckSet(name, mUser, f, true);
                // add to the list
                mListAdapter.add(deckSet.name());
                mListAdapter.notifyDataSetChanged();
                mDeckSets.put(deckSet.name(), deckSet);
                f.delete();
            }
        }
    };

    private AdapterView.OnItemLongClickListener mRemoveListener = new AdapterView.OnItemLongClickListener() {

        @Override
        public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
            String name = mListAdapter.getItem(position);
            try {
                DeckSet d = mDeckSets.get(name);
                d.remove();
                d.delete();
                mListAdapter.remove(name);
                mListAdapter.notifyDataSetChanged();
                mDeckSets.remove(name);
            } catch (NullPointerException e) {
                Toast.makeText(DeckSetActivity.this, "Unable to remove " + name, Toast.LENGTH_SHORT).show();
            }
            return true;
        }
    };

    private AdapterView.OnItemClickListener mOpenListener = new AdapterView.OnItemClickListener() {

        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

        }
    };
}
