package net.sectorsoftware.ygo.deck;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.Closeable;
import java.io.IOException;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

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
}
