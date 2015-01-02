package net.sectorsoftware.ygo.deck;

import java.util.ArrayList;

import net.sectorsoftware.ygo.data.DataTypes.StaticCardData;
import net.sectorsoftware.ygo.data.DataTypes.CardType;
import net.sectorsoftware.ygo.data.DataTypes.MonsterType;
import net.sectorsoftware.ygo.data.DataTypes.SpellType;
import net.sectorsoftware.ygo.data.DataTypes.TrapType;
import net.sectorsoftware.ygo.data.DataTypes.Attribute;
import net.sectorsoftware.ygo.data.DataTypes.Type;

public class CardSelector
{
    private native long init();
    private native ArrayList<String> execute(long p);
    private native void delete(long p);
    private native void name(long p, String like);

    public CardSelector()
    {
        mCardSelectorHandler = init();
    }

    public ArrayList<String> execute()
    {
        ArrayList<String> ret = execute(mCardSelectorHandler);
        delete(mCardSelectorHandler);
        mCardSelectorHandler = 0;
        return ret;
    }

    public CardSelector name(String like)
    {
        name(mCardSelectorHandler, like);
        return this;
    }

    private long mCardSelectorHandler = 0;
}
