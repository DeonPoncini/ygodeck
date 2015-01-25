package net.sectorsoftware.ygo.deck;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.Closeable;
import java.io.IOException;
import java.lang.RuntimeException;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class UserTest
{
    static {
        System.loadLibrary("ygodeck-jni");
    }

    static class User_Fixture implements Closeable
    {
        public User_Fixture()
        {
            DB.setPath(System.getenv("CARD_DB_PATH"));
        }

        @Override
        public void close() throws IOException
        {
        }

    }

    private User_Fixture fixture;

    @Before
    public void setup()
    {
        fixture = new User_Fixture();
    }

    @After
    public void tearDown() throws IOException
    {
        fixture.close();
        fixture = null;
    }

    @Test
    public void Create()
    {
        User user = new User("Test", true);
        assertEquals(user.name(), "Test");
        user.delete();
    }

    @Test
    public void Open()
    {
        User user = new User("OpenTest", true);
        user.delete();
        User user2 = new User("OpenTest");
        assertEquals(user2.name(), "OpenTest");
        user.delete();
    }

    @Test
    public void Unknown()
    {
        try {
            User user = new User("NotKnown");
            assertEquals(0, 1);
        } catch (RuntimeException e) {
            assertEquals(1, 1);
        }
    }

    @Test
    public void Remove()
    {
        User user = new User("RemoveTest", true);
        assertEquals(user.name(), "RemoveTest");
        user.remove();
        user.delete();
        try {
            User user2 = new User("RemoveTest");
            assertEquals(0, 1);
        } catch (RuntimeException e) {
            assertEquals(1, 1);
        }
    }

}
