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
    private native StaticCardData query(long p, String name);
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
        if (mCardSelectorHandle == 0) {
            mCardSelectorHandle = init();
        }
    }

    public CardSelector()
    {
        checkInit();
    }

    public void delete()
    {
        if (mCardSelectorHandle != 0) {
            delete(mCardSelectorHandle);
            mCardSelectorHandle = 0;
        }
    }

    public StaticCardData query(String name)
    {
        return query(mCardSelectorHandle, name);
    }

    public ArrayList<String> execute()
    {
        checkInit();
        ArrayList<String> ret = execute(mCardSelectorHandle);
        delete();
        return ret;
    }

    public CardSelector name(String like)
    {
        checkInit();
        name(mCardSelectorHandle, like);
        return this;
    }

    public CardSelector cardType(CardType ct)
    {
        checkInit();
        cardType(mCardSelectorHandle, ct.ordinal());
        return this;
    }

    public CardSelector attribute(Attribute a)
    {
        checkInit();
        attribute(mCardSelectorHandle, a.ordinal());
        return this;
    }

    public CardSelector monsterType(MonsterType mt)
    {
        checkInit();
        monsterType(mCardSelectorHandle, mt.ordinal());
        return this;
    }

    public CardSelector type(Type t)
    {
        checkInit();
        type(mCardSelectorHandle, t.ordinal());
        return this;
    }

    public CardSelector level(int l, Operator op)
    {
        checkInit();
        level(mCardSelectorHandle, l, op.ordinal());
        return this;
    }

    public CardSelector attack(int a, Operator op)
    {
        checkInit();
        attack(mCardSelectorHandle, a, op.ordinal());
        return this;
    }

    public CardSelector defense(int d, Operator op)
    {
        checkInit();
        defense(mCardSelectorHandle, d, op.ordinal());
        return this;
    }

    public CardSelector lpendulum(int d, Operator op)
    {
        checkInit();
        lpendulum(mCardSelectorHandle, d, op.ordinal());
        return this;
    }

    public CardSelector rpendulum(int d, Operator op)
    {
        checkInit();
        rpendulum(mCardSelectorHandle, d, op.ordinal());
        return this;
    }

    public CardSelector spellType(SpellType st)
    {
        checkInit();
        spellType(mCardSelectorHandle, st.ordinal());
        return this;
    }

    public CardSelector trapType(TrapType tt)
    {
        checkInit();
        trapType(mCardSelectorHandle, tt.ordinal());
        return this;
    }

    private long mCardSelectorHandle = 0;
}
