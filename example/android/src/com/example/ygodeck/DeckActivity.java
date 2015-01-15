package com.example.ygodeck;

import android.content.Intent;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.ViewPager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import net.sectorsoftware.ygo.data.DataTypes;
import net.sectorsoftware.ygo.deck.CardSelector;
import net.sectorsoftware.ygo.deck.DeckSet;
import net.sectorsoftware.ygo.deck.Format;
import net.sectorsoftware.ygo.deck.User;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;


public class DeckActivity extends ActionBarActivity implements ActionBar.TabListener,
        MainDeckFragment.OnFragmentInteractionListener,
        SideDeckFragment.OnFragmentInteractionListener,
        ExtraDeckFragment.OnFragmentInteractionListener {

    private ViewPager mViewPager;
    private DeckAdapter mDeckAdapter;
    private ActionBar mActionBar;

    private String[] mTabs = { "Main", "Side", "Extra" };

    private DeckSet mDeckSet;
    private int mCurrentDeck = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_deck);

        // get out the deck set information
        Intent intent = getIntent();
        String deckSetName = intent.getStringExtra(DeckSetActivity.EXTRA_DECKSET_NAME);
        String userName = intent.getStringExtra(DeckSetActivity.EXTRA_DECKSET_USER);
        DataTypes.Format format = DataTypes.Format.values()[intent.getIntExtra(DeckSetActivity.EXTRA_DECKSET_FORMAT, 0)];
        String formatDate = intent.getStringExtra(DeckSetActivity.EXTRA_DECKSET_FORMAT_DATE);

        User user = new User(userName);
        Format f =  new Format(format, formatDate);
        mDeckSet = new DeckSet(deckSetName, user, f);
        user.delete();
        f.delete();

        mViewPager = (ViewPager) findViewById(R.id.deck_pager);
        mActionBar = getSupportActionBar();
        mDeckAdapter = new DeckAdapter(getSupportFragmentManager());

        mViewPager.setAdapter(mDeckAdapter);
        mActionBar.setHomeButtonEnabled(false);
        mActionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);

        for (String tab : mTabs) {
            mActionBar.addTab(mActionBar.newTab().setText(tab).setTabListener(this));
        }

        mViewPager.setOnPageChangeListener(mPageChangeListener);

        mViewPager.setCurrentItem(mCurrentDeck);

        Button addButton = (Button) findViewById(R.id.deck_add_button);
        addButton.setOnClickListener(mOnAddCard);
        Button removeButton = (Button) findViewById(R.id.deck_remove_button);
        removeButton.setOnClickListener(mOnRemoveCard);
        Button exportButton = (Button) findViewById(R.id.deck_export_button);
        exportButton.setOnClickListener(mOnExport);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mDeckSet.delete();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_deck, menu);
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

    @Override
    public void onTabSelected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
        mCurrentDeck = tab.getPosition();
        mViewPager.setCurrentItem(mCurrentDeck);
    }

    @Override
    public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {

    }

    @Override
    public void onTabReselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {

    }

    @Override
    public void onSideSelected(String id) {

    }

    @Override
    public void onExtraSelected(String id) {

    }


    @Override
    public void onMainSelected(String id) {

    }

    @Override
    public void onMainLoaded(DeckContent d) {
        Map<DataTypes.DeckType,List<DataTypes.StaticCardData>> cards = mDeckSet.cards();
        List<DataTypes.StaticCardData> mainCards = cards.get(DataTypes.DeckType.MAIN);

        ArrayList<String> mainCardString = new ArrayList<String>();
        for(DataTypes.StaticCardData s : mainCards) {
            mainCardString.add(s.name);
        }

        d.addCards(mainCardString);
    }

    @Override
    public void onSideLoaded(DeckContent d) {
        Map<DataTypes.DeckType,List<DataTypes.StaticCardData>> cards = mDeckSet.cards();
        List<DataTypes.StaticCardData> sideCards = cards.get(DataTypes.DeckType.SIDE);

        ArrayList<String> sideCardString = new ArrayList<String>();
        for(DataTypes.StaticCardData s : sideCards) {
            sideCardString.add(s.name);
        }

        d.addCards(sideCardString);

    }

    @Override
    public void onExtraLoaded(DeckContent d) {
        Map<DataTypes.DeckType,List<DataTypes.StaticCardData>> cards = mDeckSet.cards();
        List<DataTypes.StaticCardData> extraCards = cards.get(DataTypes.DeckType.EXTRA);


        ArrayList<String> extraCardString = new ArrayList<String>();
        for(DataTypes.StaticCardData s : extraCards) {
            extraCardString.add(s.name);
        }

        d.addCards(extraCardString);
    }

    private ViewPager.OnPageChangeListener mPageChangeListener = new ViewPager.OnPageChangeListener() {
        @Override
        public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

        }

        @Override
        public void onPageSelected(int position) {
            mActionBar.setSelectedNavigationItem(position);
        }

        @Override
        public void onPageScrollStateChanged(int state) {

        }
    };

    private View.OnClickListener mOnAddCard = new View.OnClickListener() {

        @Override
        public void onClick(View v) {
            Intent intent = new Intent(DeckActivity.this, CardSearcher.class);
            startActivity(intent);
        }
    };

    private View.OnClickListener mOnRemoveCard = new View.OnClickListener() {

        @Override
        public void onClick(View v) {

        }
    };

    private View.OnClickListener mOnExport = new View.OnClickListener() {

        @Override
        public void onClick(View v) {

        }
    };
}
