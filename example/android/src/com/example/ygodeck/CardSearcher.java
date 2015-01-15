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
import android.widget.Toast;

import net.sectorsoftware.ygo.deck.CardSelector;

import java.util.List;


public class CardSearcher extends ActionBarActivity {

    private EditText mEditText;
    private ListView mListView;
    private ArrayAdapter<String> mListAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_card_searcher);

        Button search = (Button) findViewById(R.id.card_search_button);
        search.setOnClickListener(mOnSearchCard);
        mEditText = (EditText) findViewById(R.id.card_search_edit);

        mListView = (ListView) findViewById(R.id.card_result_list);
        mListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        mListView.setAdapter(mListAdapter);
        mListView.setOnItemClickListener(mInspectListener);
        mListView.setOnItemLongClickListener(mAddCardListener);
    }


    private View.OnClickListener mOnSearchCard = new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            // search for the card text
            String search = mEditText.getText().toString();
            if (search.isEmpty()) {
                Toast.makeText(CardSearcher.this, "Please enter a search term",
                        Toast.LENGTH_SHORT).show();
            } else {
                CardSelector cs = new CardSelector();
                List<String> results = cs.name(search).execute();

                if (results.isEmpty()) {
                    Toast.makeText(CardSearcher.this, "No results for " + search,
                            Toast.LENGTH_SHORT).show();
                } else {
                    mListAdapter.clear();
                    mListAdapter.addAll(results);
                    mListAdapter.notifyDataSetChanged();
                    mEditText.clearFocus();
                    mListView.requestFocus();
                }
            }
        }
    };

    private AdapterView.OnItemLongClickListener mAddCardListener = new AdapterView.OnItemLongClickListener() {

        @Override
        public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
            String name = mListAdapter.getItem(position);
            Intent returnIntent = new Intent();
            returnIntent.putExtra(DeckActivity.RESULT_CARD_NAME,name);
            setResult(RESULT_OK,returnIntent);
            finish();
            return true;
        }
    };

    private AdapterView.OnItemClickListener mInspectListener = new AdapterView.OnItemClickListener() {

        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            Intent intent = new Intent(CardSearcher.this, CardViewer.class);
            String cardName = mListAdapter.getItem(position);
            intent.putExtra(CardViewer.EXTRA_CARD_NAME, cardName);
            startActivity(intent);
        }
    };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_card_searcher, menu);
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
