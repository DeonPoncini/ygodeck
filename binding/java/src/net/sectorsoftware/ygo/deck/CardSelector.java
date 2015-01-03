package net.sectorsoftware.ygo.deck;

import java.util.ArrayList;

import net.sectorsoftware.mindbw.DataTypes.Operator;
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
    private native void cardType(long p, int ct);
    private native void attribute(long p, int a);
    private native void monsterType(long p, int mt);
    private native void type(long p, int t);
    private native void level(long p, int l, int op);
    private native void attack(long p, int a, int op);
    private native void defense(long p, int d, int op);
    private native void lpendulum(long p, int l, int op);
    private native void rpendulum(long p, int r, int op);
    private native void spellType(long p, int st);
    private native void trapType(long p, int tt);

    private void checkInit()
    {
        if (mCardSelectorHandler == 0) {
            mCardSelectorHandler = init();
        }
    }

    public CardSelector()
    {
        checkInit();
    }

    public void delete()
    {
        if (mCardSelectorHandler != 0) {
            delete(mCardSelectorHandler);
            mCardSelectorHandler = 0;
        }
    }

    public ArrayList<String> execute()
    {
        checkInit();
        ArrayList<String> ret = execute(mCardSelectorHandler);
        delete();
        return ret;
    }

    public CardSelector name(String like)
    {
        checkInit();
        name(mCardSelectorHandler, like);
        return this;
    }

    public CardSelector cardType(CardType ct)
    {
        checkInit();
        cardType(mCardSelectorHandler, ct.ordinal());
        return this;
    }

    public CardSelector attribute(Attribute a)
    {
        checkInit();
        attribute(mCardSelectorHandler, a.ordinal());
        return this;
    }

    public CardSelector monsterType(MonsterType mt)
    {
        checkInit();
        monsterType(mCardSelectorHandler, mt.ordinal());
        return this;
    }

    public CardSelector type(Type t)
    {
        checkInit();
        type(mCardSelectorHandler, t.ordinal());
        return this;
    }

    public CardSelector level(int l, Operator op)
    {
        checkInit();
        level(mCardSelectorHandler, l, op.ordinal());
        return this;
    }

    public CardSelector attack(int a, Operator op)
    {
        checkInit();
        attack(mCardSelectorHandler, a, op.ordinal());
        return this;
    }

    public CardSelector defense(int d, Operator op)
    {
        checkInit();
        defense(mCardSelectorHandler, d, op.ordinal());
        return this;
    }

    public CardSelector lpendulum(int d, Operator op)
    {
        checkInit();
        lpendulum(mCardSelectorHandler, d, op.ordinal());
        return this;
    }

    public CardSelector rpendulum(int d, Operator op)
    {
        checkInit();
        rpendulum(mCardSelectorHandler, d, op.ordinal());
        return this;
    }

    public CardSelector spellType(SpellType st)
    {
        checkInit();
        spellType(mCardSelectorHandler, st.ordinal());
        return this;
    }

    public CardSelector trapType(TrapType tt)
    {
        checkInit();
        trapType(mCardSelectorHandler, tt.ordinal());
        return this;
    }

    private long mCardSelectorHandler = 0;
}
