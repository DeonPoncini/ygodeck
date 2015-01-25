package net.sectorsoftware.ygo.deck;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.Closeable;
import java.io.IOException;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import net.sectorsoftware.mindbw.DataTypes.Operator;
import net.sectorsoftware.ygo.data.DataTypes.StaticCardData;
import net.sectorsoftware.ygo.data.DataTypes.CardType;
import net.sectorsoftware.ygo.data.DataTypes.MonsterType;
import net.sectorsoftware.ygo.data.DataTypes.SpellType;
import net.sectorsoftware.ygo.data.DataTypes.TrapType;
import net.sectorsoftware.ygo.data.DataTypes.Attribute;
import net.sectorsoftware.ygo.data.DataTypes.Type;

public class CardSelectorTest
{
    static {
        System.loadLibrary("ygodeck-jni");
    }

    static class CardSelector_Fixture implements Closeable
    {
        public CardSelector_Fixture()
        {
            DB.setPath(System.getenv("CARD_DB_PATH"));
            cardSelector = new CardSelector();
        }

        @Override
        public void close() throws IOException
        {
            cardSelector.delete();
            cardSelector = null;
        }

    }

    private CardSelector_Fixture fixture;
    private static CardSelector cardSelector;

    @Before
    public void setup()
    {
        fixture = new CardSelector_Fixture();
    }

    @After
    public void tearDown() throws IOException
    {
        fixture.close();
        fixture = null;
    }

    @Test
    public void Name()
    {
        List<String> clist = cardSelector.name("Black").execute();
        for (String c : clist) {
            assertTrue(c.contains("Black"));
        }
    }

    @Test
    public void CardType()
    {
        List<String> clist = cardSelector.name("Blue").cardType(
                CardType.SPELL).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.cardType, CardType.SPELL);
        }
    }

    @Test
    public void Attribute()
    {
        List<String> clist = cardSelector.name("Dragon").attribute(
                Attribute.DARK).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.attribute, Attribute.DARK);
        }
    }

    @Test
    public void MonsterType()
    {
        List<String> clist = cardSelector.name("Wind").monsterType(
                MonsterType.XYZ).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.monsterType, MonsterType.XYZ);
        }
    }

    @Test
    public void Type()
    {
        List<String> clist = cardSelector.name("saurus").type(
                Type.DINOSAUR).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.type, Type.DINOSAUR);
        }
    }

    @Test
    public void Level()
    {
        List<String> clist = cardSelector.level(11, Operator.EQ).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.level, 11);
        }
    }

    @Test
    public void Attack()
    {
        List<String> clist = cardSelector.type(Type.DRAGON).attack(1000,
                Operator.LT).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertTrue(scd.attack < 1000);
        }
    }

    @Test
    public void Defense()
    {
        List<String> clist = cardSelector.type(Type.ROCK).defense(100,
                Operator.LTE).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertTrue(scd.defense <= 100);
        }
    }

    @Test
    public void Pendulum()
    {
        List<String> clist = cardSelector.lpendulum(8, Operator.GTE).
            rpendulum(8, Operator.GTE).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertTrue(scd.lpendulum >= 8);
            assertTrue(scd.rpendulum >= 8);
        }
    }

    @Test
    public void SpellType()
    {
        List<String> clist = cardSelector.name("Sky").spellType(
                SpellType.FIELD).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.spellType, SpellType.FIELD);
        }
    }

    @Test
    public void TrapType()
    {
        List<String> clist = cardSelector.name("Solemn").trapType(
                TrapType.COUNTER).execute();
        for (String c : clist) {
            StaticCardData scd = cardSelector.query(c);
            assertEquals(scd.trapType, TrapType.COUNTER);
        }
    }
}
