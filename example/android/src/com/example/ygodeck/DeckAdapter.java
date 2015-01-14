package com.example.ygodeck;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class DeckAdapter extends FragmentPagerAdapter {

    public DeckAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        switch (position) {
            case 0: return new MainDeckFragment();
            case 1: return new SideDeckFragment();
            case 2: return new ExtraDeckFragment();
        }
        return null;
    }

    @Override
    public int getCount() {
        return 3;
    }
}
