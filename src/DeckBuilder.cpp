#include "DeckBuilder.h"

#include "Serialize.h"

namespace ygo
{

DeckBuilder::DeckBuilder() :
    mDB("card.db")
{
}

DeckBuilder::DeckCreateError
DeckBuilder::create(const std::string& name, Format format,
            const std::string& formatDate)
{
    mName = name;
    mFormatDate = formatDate;
    mFormat = format;
    // check if a deck by that name already exists
    if (exists())
    {
        return DeckCreateError::ALREADY_EXISTS;
    }

    // check the format date is legal
    if (!formatExists())
    {
        return DeckCreateError::INVALID_FORMAT;
    }

    // create the primary, side and extra deck ids
    createDeck(DeckType::MAIN);
    createDeck(DeckType::SIDE);
    createDeck(DeckType::EXTRA);

    // extract the IDs to make a link
    mMainID = deckID(DeckType::MAIN);
    mSideID = deckID(DeckType::SIDE);
    mExtraID = deckID(DeckType::EXTRA);

    // create the deck set entry
    mDB.insert("deck_set",
            std::string("NULL,") +
            DBEsc(name) + "," +
            DBEsc(fromFormat(format)) + "," +
            DBEsc(formatDate) + "," +
            DBEsc(mMainID) + "," +
            DBEsc(mSideID) + "," +
            DBEsc(mExtraID),
            [&](DB::DataMap){});

    return DeckCreateError::OK;
}

DeckBuilder::DeckError
DeckBuilder::addCard(DeckType deck, const std::string& name)
{
    // get the card id
    auto id = cardID(name);
    // check how many of this card can be put into the deck
    auto count = cardCount(id);
    // check how many of this card we already have
    auto exist = cardCheck(id,mMainID);
    exist += cardCheck(id,mSideID);
    exist += cardCheck(id,mExtraID);

    if (count <= exist)
    {
        return DeckError::LIMIT_REACHED;
    }

    // otherwise insert into the deck
    mDB.insert("deck_to_cards",
            std::string("NULL, ") + deckTypeToId(deck) + "," + id,
            [&](DB::DataMap){});

    return DeckError::OK;
}

DeckBuilder::FormatType
DeckBuilder::formats() const
{
    static FormatType ft = {
        fromFormat(Format::TRADITIONAL),
        fromFormat(Format::ADVANCED)
    };
    return ft;
}

DeckBuilder::FormatType
DeckBuilder::formatDates()
{
    FormatType ft;
    // get all the format dates
    mDB.select("DISTINCT name","formats","1",
            [&](DB::DataMap data)
            {
                for (auto&& kv : data)
                {
                    if (kv.second == fromLimitation(Limitation::ILLEGAL))
                    {
                        continue;
                    }
                    ft.push_back(kv.second);
                }
            });
    return ft;
}

std::string DeckBuilder::deckID(DeckType deckType)
{
    std::string id;
    mDB.select("deck_id","deck",
            std::string("name = ") + DBEsc(mName) + " AND type = " +
            DBEsc(fromDeckType(deckType)), [&](DB::DataMap data)
            {
                for (auto&& kv : data)
                {
                    id = kv.second;
                }
            });
    return id;
}

void DeckBuilder::createDeck(DeckType deckType)
{
    mDB.insert("deck",std::string("NULL,") + DBEsc(mName) + "," +
            DBEsc(fromDeckType(deckType)), [&](DB::DataMap){});
}

bool DeckBuilder::exists()
{
    auto exists = false;
    mDB.select("name","deck_set",
            std::string("name = ") + DBEsc(mName),
            [&](DB::DataMap data)
            {
                if (data.size() > 0)
                {
                    exists = true;
                }
            });
    return exists;
}

bool DeckBuilder::formatExists()
{
    auto formatExists = true;
    mDB.select("format_id", "formats",
            std::string("name = ") + DBEsc(mFormatDate),
            [&](DB::DataMap data)
            {
                if (data.size() == 0)
                {
                    formatExists = false;
                }
            });
    return formatExists;
}

std::string DeckBuilder::cardID(const std::string& name)
{
    std::string id;
    mDB.select("card_id","card",
            std::string("name = ") + DBEsc(name),
            [&](DB::DataMap data)
            {
                for (auto&& kv : data)
                {
                    id = kv.second;
                }
            });
    return id;
}

int DeckBuilder::cardCount(const std::string& id)
{
    // look up this card in the formats database
    auto count = 0;
    mDB.select("card_status","formats",
            std::string("card_id = ") + DBEsc(id) + " AND " +
            "(name = " + DBEsc(mFormatDate) + " OR name = "
            + DBEsc(fromLimitation(Limitation::ILLEGAL)) + ")"
            ,[&](DB::DataMap data)
            {
                if (data.size() == 0)
                {
                    count = limitation(Limitation::UNLIMITED);
                }
                else
                {
                    for (auto&& kv : data)
                    {
                        count = limitation(toLimitation(kv.second));
                    }
                }
            });
    return count;
}

int DeckBuilder::cardCheck(const std::string& cardid,
        const std::string& deckid)
{
    auto count = 0;
    mDB.select("relation_id","deck_to_cards",
            "deck_id = " + DBEsc(deckid) + " AND card_id = " + DBEsc(cardid),
            [&](DB::DataMap data)
            {
                count = data.size();
            });
    return count;
}

std::string DeckBuilder::deckTypeToId(DeckType deckType) const
{
    switch (deckType)
    {
        case DeckType::MAIN: return mMainID;
        case DeckType::SIDE: return mSideID;
        case DeckType::EXTRA: return mExtraID;
    }
}

int DeckBuilder::limitation(Limitation limitation) const
{
    switch (limitation)
    {
        case Limitation::ILLEGAL: return 0;
        case Limitation::FORBIDDEN:
        {
            switch (mFormat)
            {
                case Format::TRADITIONAL: return 1;
                case Format::ADVANCED: return 0;
            }
        }
        case Limitation::LIMITED: return 1;
        case Limitation::SEMILIMITED: return 2;
        case Limitation::UNLIMITED: return 3;
    }
}

}
