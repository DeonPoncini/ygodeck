package net.sectorsoftware.ygo.deck;

import java.util.ArrayList;

import net.sectorsoftware.ygo.data.DataTypes;

public class Format
{
    private native long init(int format, String formatDate);
    private native void delete(long p);
    private native int format(long p);
    private native String formatDate(long p);
    private native int cardCount(long p, String card);
    public static native ArrayList<String> formatDates();
    public static native ArrayList<String> formats();

    public Format(DataTypes.Format format, String formatDate)
    {
        mFormatHandle = init(format.ordinal(), formatDate);
    }

    public void delete()
    {
        if (mFormatHandle != 0) {
            delete(mFormatHandle);
            mFormatHandle = 0;
        }
    }

    public DataTypes.Format format()
    {
        assert mFormatHandle != 0;
        return DataTypes.Format.values()[format(mFormatHandle)];
    }

    public String formatDate()
    {
        assert mFormatHandle != 0;
        return formatDate(mFormatHandle);
    }

    public int cardCount(String card)
    {
        assert mFormatHandle != 0;
        return cardCount(mFormatHandle, card);
    }

    private long mFormatHandle;
}
