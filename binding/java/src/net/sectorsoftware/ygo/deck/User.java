package net.sectorsoftware.ygo.deck;

import java.util.ArrayList;

public class User
{
    private native long init(String name, boolean create);
    private native void delete(long p);
    private native String name(long p);
    private native String id(long p);
    private native ArrayList<DeckSet> deckSets(long p);
    private native void remove(long p);

    public User(String name)
    {
        this(name, false);
    }

    public User(String name, boolean create)
    {
        mUserHandle = init(name, create);
    }

    public void delete()
    {
        if (mUserHandle != 0) {
            delete(mUserHandle);
            mUserHandle = 0;
        }
    }

    public String name()
    {
        assert mUserHandle != 0;
        return name(mUserHandle);
    }

    public String id()
    {
        assert mUserHandle != 0;
        return name(mUserHandle);
    }

    public ArrayList<DeckSet> deckSets()
    {
        assert mUserHandle != 0;
        return deckSets(mUserHandle);
    }

    public void remove()
    {
        assert mUserHandle != 0;
        remove(mUserHandle);
    }

    private long mUserHandle = 0;
}
