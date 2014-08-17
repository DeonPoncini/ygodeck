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
            DBList({
                name,
                fromFormat(format),
                formatDate,
                mMainID,
                mSideID,
                mExtraID
                }));

    return DeckCreateError::OK;
}

DeckBuilder::DeckOpenError
DeckBuilder::open(const std::string& name)
{
    mName = name;
    if (!exists())
    {
        return DeckOpenError::DOES_NOT_EXIST;
    }
    // select the rest of the information
    mDB.select(DBKeyList({"format","format_date"}),"deck_set",
            DBPair("name",mName),
            [&](DB::DataMap data)
            {
                mFormat = toFormat(data["format"]);
                mFormatDate = data["format_date"];
            });

    mMainID = deckID(DeckType::MAIN);
    mSideID = deckID(DeckType::SIDE);
    mExtraID = deckID(DeckType::EXTRA);

    return DeckOpenError::OK;
}

DeckBuilder::DeckError
DeckBuilder::addCard(DeckType deck, const std::string& name)
{
    // check if we are at the limit
    auto cards = getCards(deck);
    if (cards.size() >= DeckLimits[deck].second)
    {
        return DeckError::DECK_FULL;
    }
    // get the card id
    auto id = cardID(name);
    // check how many of this card can be put into the deck
    auto count = cardCount(name);
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
            DBList({
                deckTypeToId(deck),
                id
                }));

    return DeckError::OK;
}

std::vector<std::string> DeckBuilder::getCards(DeckType deck)
{
    std::vector<std::string> ids;
    mDB.select("card_id","deck_to_cards",
            DBPair("deck_id",deckTypeToId(deck)),
            [&](DB::DataMap data)
            {
                ids.push_back(data["card_id"]);
            });

    std::vector<std::string> names;
    for (auto&& i : ids)
    {
        mDB.select("name","card",
                DBPair("card_id",i),
                [&](DB::DataMap data)
                {
                    names.push_back(data["name"]);
                });
    }
    return names;
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
    mDB.select(DBUnique("name"),"formats",DBTrue(),
            [&](DB::DataMap data)
            {
                auto format = data["name"];
                if (format == fromLimitation(Limitation::ILLEGAL))
                {
                    return;
                }
                ft.push_back(format);
            });
    return ft;
}

std::string DeckBuilder::deckID(DeckType deckType)
{
    std::string id;
    mDB.select("deck_id","deck",
            DBAnd({DBPair("name",mName),
                DBPair("type",fromDeckType(deckType))})
            , [&](DB::DataMap data)
            {
                id = data["deck_id"];
            });
    return id;
}

void DeckBuilder::createDeck(DeckType deckType)
{
    mDB.insert("deck",
            DBList({
                mName,
                fromDeckType(deckType)
                }));
}

bool DeckBuilder::exists()
{
    auto exists = false;
    mDB.select("name","deck_set",
            DBPair("name",mName),
            [&](DB::DataMap data)
            {
                exists = true;
            });
    return exists;
}

bool DeckBuilder::formatExists()
{
    auto formatExists = false;
    mDB.select("format_id", "formats",
            DBPair("name",mFormatDate),
            [&](DB::DataMap data)
            {
                formatExists = true;
            });
    return formatExists;
}

std::string DeckBuilder::cardID(const std::string& name)
{
    std::string id;
    mDB.select("card_id","card",
            DBPair("name",name),
            [&](DB::DataMap data)
            {
                id = data["card_id"];
            });
    return id;
}

int DeckBuilder::cardCount(const std::string& name)
{
    // look up this card in the formats database
    auto count = 0;
    auto callback = false;
    mDB.select("card_status","formats",
            DBAnd({
                DBPair("card_id",name),
                DBOr({
                    DBPair("name",mFormatDate),
                    DBPair("name",fromLimitation(Limitation::ILLEGAL))})}),
            [&](DB::DataMap data)
            {
                callback = true;
                count = limitation(toLimitation(data["card_status"]));
            });
    if (!callback)
    {
        count = limitation(Limitation::UNLIMITED);
    }
    return count;
}

int DeckBuilder::cardCheck(const std::string& cardid,
        const std::string& deckid)
{
    auto count = 0;
    mDB.select("relation_id","deck_to_cards",
            DBAnd({
                DBPair("deck_id",deckid),
                DBPair("card_id",cardid)}),
            [&](DB::DataMap data)
            {
                count += data.size();
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
