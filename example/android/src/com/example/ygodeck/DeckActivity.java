package com.example.ygodeck;

import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.ViewPager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;


public class DeckActivity extends ActionBarActivity implements ActionBar.TabListener,
        MainDeckFragment.OnFragmentInteractionListener,
        SideDeckFragment.OnFragmentInteractionListener,
        ExtraDeckFragment.OnFragmentInteractionListener {

    private ViewPager mViewPager;
    private DeckAdapter mDeckAdapter;
    private ActionBar mActionBar;

    private String[] mTabs = { "Main", "Side", "Extra" };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_deck);

        mViewPager = (ViewPager) findViewById(R.id.deck_pager);
        mActionBar = getSupportActionBar();
        mDeckAdapter = new DeckAdapter(getSupportFragmentManager());

        mViewPager.setAdapter(mDeckAdapter);
        mActionBar.setHomeButtonEnabled(false);
        mActionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);

        for (String tab : mTabs) {
            mActionBar.addTab(mActionBar.newTab().setText(tab).setTabListener(this));
        }

        mViewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {

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
        });
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
        mViewPager.setCurrentItem(tab.getPosition());
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
}
