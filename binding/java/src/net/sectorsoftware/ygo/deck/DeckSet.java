package net.sectorsoftware.ygo.deck;

import java.util.Map;
import java.util.List;

import net.sectorsoftware.ygo.data.DataTypes.StaticCardData;
import net.sectorsoftware.ygo.data.DataTypes.DeckType;
import net.sectorsoftware.ygo.deck.DataTypes.DeckError;

public class DeckSet
{
    private native long init(String name, long user, long format);
    private native long init_create(String name, long user, long format);
    private native void delete(long p);
    private native String name(long p);
    private native long format(long p);
    private native int addCard(long p, int deckType, String name);
    private native Map<DeckType, List<StaticCardData>> cards(long p);
    private native void deleteCard(long p, int deckType, String name);
    private native void remove(long p);
    private native boolean validate(long p);

    public DeckSet(String name, User user, Format format)
    {
        this(name, user, format, false);
    }

    public DeckSet(String name, User user, Format format, boolean create)
    {
        if (create) {
            mDeckSetHandle = init_create(name, user.getHandle(),
                    format.getHandle());
        } else {
            mDeckSetHandle = init(name, user.getHandle(), format.getHandle());
        }
    }

    /* package */ DeckSet(long handle)
    {
        mDeckSetHandle = handle;
    }

    public void delete()
    {
        if (mDeckSetHandle != 0) {
            delete(mDeckSetHandle);
            mDeckSetHandle = 0;
        }
    }

    public String name()
    {
        assert mDeckSetHandle != 0;
        return name(mDeckSetHandle);
    }

    public Format format()
    {
        assert mDeckSetHandle != 0;
        return new Format(format(mDeckSetHandle));
    }

    public DeckError addCard(DeckType deckType, String name)
    {
        assert mDeckSetHandle != 0;
        return DeckError.values()[addCard(mDeckSetHandle,
                deckType.ordinal(), name)];
    }

    public Map<DeckType, List<StaticCardData>> cards()
    {
        assert mDeckSetHandle != 0;
        return cards(mDeckSetHandle);
    }

    public void deleteCard(DeckType deckType, String name)
    {
        assert mDeckSetHandle != 0;
        deleteCard(mDeckSetHandle, deckType.ordinal(), name);
    }

    public void remove()
    {
        assert mDeckSetHandle != 0;
        remove(mDeckSetHandle);
    }

    public boolean validate()
    {
        assert mDeckSetHandle != 0;
        return validate(mDeckSetHandle);
    }

    private long mDeckSetHandle;
}
