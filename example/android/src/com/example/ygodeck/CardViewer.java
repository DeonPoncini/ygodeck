package com.example.ygodeck;

import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import net.sectorsoftware.ygo.data.DataTypes;
import net.sectorsoftware.ygo.deck.CardSelector;


public class CardViewer extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_card_viewer);

        ListView listView = (ListView) findViewById(R.id.card_viewer_list);
        ArrayAdapter<String> listAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        listView.setAdapter(listAdapter);

        // get the card results
        Intent intent = getIntent();
        String cardName = intent.getStringExtra(CardSearcher.EXTRA_CARD_NAME);
        CardSelector cs = new CardSelector();
        DataTypes.StaticCardData s = cs.query(cardName);
        cs.delete();

        listAdapter.add(s.name);
        listAdapter.add("CardType: " + s.cardType);
        listAdapter.add("Attribute: " + s.attribute);
        listAdapter.add("MonsterType: " + s.monsterType);
        listAdapter.add("Type: " + s.type);
        listAdapter.add("MonsterAbility: " + s.monsterAbility);
        listAdapter.add("Level: " + s.level);
        listAdapter.add("Attack: " + s.attack);
        listAdapter.add("Defense: " + s.defense);
        listAdapter.add("LPendulum: " + s.lpendulum);
        listAdapter.add("RPendulum: " + s.rpendulum);
        listAdapter.add("SpellType: " + s.spellType);
        listAdapter.add("TrapType: " + s.trapType);
        listAdapter.add(s.text);

        listAdapter.notifyDataSetChanged();
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_card_viewer, menu);
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
