#include "DBDeckSet.h"

#include "DB.h"
#include "DBCommon.h"
#include "Serialize.h"
#include <stdexcept>

namespace ygo
{

std::string cardID(const std::string& name)
{
    std::string id;
    DB db(DBPATH);
    db.select("card_id","card",
            DBPair("name",name),
            [&](DB::DataMap data)
            {
                id = data["card_id"];
            });
    return id;
}

int cardCheck(const std::string& cardid, const std::string& deckid)
{
    auto count = 0;
    DB db(DBPATH);
    db.select("relation_id","deck_to_cards",
            DBAnd({
                DBPair("deck_id",deckid),
                DBPair("card_id",cardid)}),
            [&](DB::DataMap data)
            {
                count += data.size();
            });
    return count;
}

MonsterType monsterType(const std::string& name)
{
    std::string mtype;
    DB db(DBPATH);
    db.select("monsterType","card",
            DBPair("name",name),
            [&](DB::DataMap data)
            {
                mtype = data["monsterType"];
            });
    return toMonsterType(mtype);
}

DBDeckSet::DBDeckSet(const std::string& name, const DBUser& user,
            const DBFormat& format, bool create) :
    mName(name),
    mUser(user),
    mFormat(format)
{
    if (exists())
    {
        open();
    }
    else if (create)
    {
        DBDeckSet::create();
    }
    else
    {
        throw std::runtime_error("Deck Set " + name + " does not exist");
    }
}

bool DBDeckSet::exists()
{
    auto exists = false;
    DB db(DBPATH);
    // check if the deck named has the same format
    db.select(DBKeyList({"deck_set_id","format","format_date"}),"deck_set",
            DBPair("name",mName),
            [&](DB::DataMap data)
            {
                if (data["format"] == fromFormat(mFormat.format())
                    && data["format_date"] == mFormat.formatDate())
                {
                    exists = true;
                    mID = data["deck_set_id"];
                }
            });
    // now check that deck belongs to us
    if (exists)
    {
        auto owned = false;
        db.select("user_id","user_to_decks",
                DBPair("deck_set_id",mID),
                [&](DB::DataMap data)
                {
                    if (data["user_id"] == mUser.id())
                    {
                        owned = true;
                    }
                });
        if (owned)
        {
            return true;
        }
    }
    return false;
}

void DBDeckSet::create()
{
    // create the new decks
    mDeckMap.emplace(DeckMap::value_type{DeckType::MAIN,
            DBDeck{DeckType::MAIN}});
    mDeckMap.emplace(DeckMap::value_type{DeckType::SIDE,
            DBDeck{DeckType::SIDE}});
    mDeckMap.emplace(DeckMap::value_type{DeckType::EXTRA,
            DBDeck{DeckType::EXTRA}});

    // create the deck set entry
    DB db(DBPATH);
    mID = db.insert("deck_set",
            DBList({
                mName,
                fromFormat(mFormat.format()),
                mFormat.formatDate(),
                findDeck(DeckType::MAIN).id(),
                findDeck(DeckType::SIDE).id(),
                findDeck(DeckType::EXTRA).id(),
                }));

    // map the deck to the user
    db.insert("user_to_decks",
            DBList({mUser.id(),mID}));

}

void DBDeckSet::open()
{
    // find out the deck ids from the main id, note it is set in exists
    DB db(DBPATH);
    db.select(DBKeyList({"main_deck_id","side_deck_id","extra_deck_id"}),
            "deck_set",DBPair("deck_set_id",mID),
            [&](DB::DataMap data)
            {
                mDeckMap.emplace(DeckMap::value_type{DeckType::MAIN,
                        DBDeck{DeckType::MAIN, data["main_deck_id"]}});
                mDeckMap.emplace(DeckMap::value_type{DeckType::SIDE,
                        DBDeck{DeckType::SIDE, data["side_deck_id"]}});
                mDeckMap.emplace(DeckMap::value_type{DeckType::EXTRA,
                        DBDeck{DeckType::EXTRA, data["extra_deck_id"]}});
            });
}

DBDeck::DeckError DBDeckSet::addCard(DeckType deckType, const std::string& name)
{
    // check if we can add to any card list
    auto count = mFormat.cardCount(name);
    auto cardid = cardID(name);
    auto exist = 0;
    for (auto&& kv : mDeckMap)
    {
        exist += cardCheck(cardid,kv.second.id());
    }
    if (count <= exist)
    {
        return DBDeck::DeckError::LIMIT_REACHED;
    }
    // check if a card is valid for this deck type
    auto mtype = monsterType(name);
    if (mtype == MonsterType::FUSION ||
            mtype == MonsterType::SYNCHRO ||
            mtype == MonsterType::XYZ ||
            mtype == MonsterType::PENDULUM)
    {
        if (deckType != DeckType::EXTRA)
        {
            // not allowed
            return DBDeck::DeckError::FORBIDDEN;
        }
    }
    else
    {
        if (deckType == DeckType::EXTRA)
        {
            // only allows these sort of cards
            return DBDeck::DeckError::FORBIDDEN;
        }
    }
    return findDeck(deckType).addCard(name);
}

CardMap DBDeckSet::cards() const
{
    CardMap ret;
    for (auto&& kv : mDeckMap)
    {
        ret.emplace(CardMap::value_type{kv.first,kv.second.cards()});
    }
    return ret;
}

void DBDeckSet::deleteCard(DeckType deckType, const std::string& name)
{
    findDeck(deckType).deleteCard(name);
}

DBDeck& DBDeckSet::findDeck(DeckType deckType)
{
    return mDeckMap.find(deckType)->second;
}

const DBDeck& DBDeckSet::findDeck(DeckType deckType) const
{
    return mDeckMap.find(deckType)->second;
}

void DBDeckSet::remove()
{
    // remove all the decks and this deck set
    auto mid = findDeck(DeckType::MAIN).id();
    auto sid = findDeck(DeckType::SIDE).id();
    auto eid = findDeck(DeckType::EXTRA).id();

    DB db(DBPATH);
    db.del("deck",DBPair("deck_id",mid));
    db.del("deck",DBPair("deck_id",sid));
    db.del("deck",DBPair("deck_id",eid));
    db.del("deck_set",DBPair("deck_set_id",mID));
}

bool DBDeckSet::validate() const
{
    // check if the deck as built is valid and meets size requirements
    if ((findDeck(DeckType::MAIN).size() < DeckMin(DeckType::MAIN) ||
         (findDeck(DeckType::MAIN).size() > DeckMax(DeckType::MAIN))))
    {
        return false;
    }

    if ((findDeck(DeckType::EXTRA).size() < DeckMin(DeckType::EXTRA) ||
         (findDeck(DeckType::EXTRA).size() > DeckMax(DeckType::EXTRA))))
    {
        return false;
    }

    if ((findDeck(DeckType::SIDE).size() < DeckMin(DeckType::SIDE) ||
         (findDeck(DeckType::SIDE).size() > DeckMax(DeckType::SIDE))))
    {
        return false;
    }

    return true;
}

}
