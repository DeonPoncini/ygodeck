#include "DeckSet.h"

#include "Common.h"

#include <db/SQLite3.h>
#include <ygo/data/Serialize.h>

#include <stdexcept>

namespace ygo
{
namespace deck
{

std::string cardID(const std::string& name)
{
    std::string id;
    db::SQLite3 db(DBPATH);
    db.select("card_id","card",
            db::DBPair("name",name),
            [&](db::SQLite3::DataMap data) {
                id = data["card_id"];
            });
    return id;
}

int cardCheck(const std::string& cardid, const std::string& deckid)
{
    auto count = 0;
    db::SQLite3 db(DBPATH);
    db.select("relation_id","deck_to_cards",
            db::DBAnd({
                db::DBPair("deck_id",deckid),
                db::DBPair("card_id",cardid)}),
            [&](db::SQLite3::DataMap data) {
                count += data.size();
            });
    return count;
}

data::MonsterType monsterType(const std::string& name)
{
    std::string mtype;
    db::SQLite3 db(DBPATH);
    db.select("monsterType","card",
            db::DBPair("name",name),
            [&](db::SQLite3::DataMap data) {
                mtype = data["monsterType"];
            });
    return data::toMonsterType(mtype);
}

DeckSet::DeckSet(const std::string& name, const User& user,
        const Format& format, bool create) :
    mName(name),
    mUser(user),
    mFormat(format)
{
    if (exists()) {
        open();
    } else if (create) {
        DeckSet::create();
    } else {
        throw std::runtime_error("Deck Set " + name + " does not exist");
    }
}

bool DeckSet::exists()
{
    auto exists = false;
    db::SQLite3 db(DBPATH);
    // check if the deck named has the same format
    db.select(db::DBKeyList({"deck_set_id","format","format_date"}),"deck_set",
            db::DBPair("name",mName),
            [&](db::SQLite3::DataMap data) {
                if (data["format"] == fromFormat(mFormat.format())
                    && data["format_date"] == mFormat.formatDate()) {
                    exists = true;
                    mID = data["deck_set_id"];
                }
            });
    // now check that deck belongs to us
    if (exists) {
        auto owned = false;
        db.select("user_id","user_to_decks",
                db::DBPair("deck_set_id",mID),
                [&](db::SQLite3::DataMap data) {
                    if (data["user_id"] == mUser.id()) {
                        owned = true;
                    }
                });
        if (owned) {
            return true;
        }
    }
    return false;
}

void DeckSet::create()
{
    // create the new decks
    mDeckMap.emplace(DeckMap::value_type{data::DeckType::MAIN,
            Deck{data::DeckType::MAIN}});
    mDeckMap.emplace(DeckMap::value_type{data::DeckType::SIDE,
            Deck{data::DeckType::SIDE}});
    mDeckMap.emplace(DeckMap::value_type{data::DeckType::EXTRA,
            Deck{data::DeckType::EXTRA}});

    // create the deck set entry
    db::SQLite3 db(DBPATH);
    mID = db.insert("deck_set",
            db::DBList({
                mName,
                fromFormat(mFormat.format()),
                mFormat.formatDate(),
                findDeck(data::DeckType::MAIN).id(),
                findDeck(data::DeckType::SIDE).id(),
                findDeck(data::DeckType::EXTRA).id(),
                }));

    // map the deck to the user
    db.insert("user_to_decks",
            db::DBList({mUser.id(),mID}));

}

void DeckSet::open()
{
    // find out the deck ids from the main id, note it is set in exists
    db::SQLite3 db(DBPATH);
    db.select(db::DBKeyList({"main_deck_id","side_deck_id","extra_deck_id"}),
            "deck_set",db::DBPair("deck_set_id",mID),
            [&](db::SQLite3::DataMap data) {
                mDeckMap.emplace(DeckMap::value_type{data::DeckType::MAIN,
                        Deck{data::DeckType::MAIN, data["main_deck_id"]}});
                mDeckMap.emplace(DeckMap::value_type{data::DeckType::SIDE,
                        Deck{data::DeckType::SIDE, data["side_deck_id"]}});
                mDeckMap.emplace(DeckMap::value_type{data::DeckType::EXTRA,
                        Deck{data::DeckType::EXTRA, data["extra_deck_id"]}});
            });
}

DeckError DeckSet::addCard(data::DeckType deckType,
        const std::string& name)
{
    // check if we can add to any card list
    auto count = mFormat.cardCount(name);
    auto cardid = cardID(name);
    auto exist = 0;
    for (auto&& kv : mDeckMap) {
        exist += cardCheck(cardid,kv.second.id());
    }
    if (count <= exist) {
        return DeckError::LIMIT_REACHED;
    }
    // check if a card is valid for this deck type
    auto mtype = monsterType(name);
    if (mtype == data::MonsterType::FUSION ||
            mtype == data::MonsterType::SYNCHRO ||
            mtype == data::MonsterType::XYZ ||
            mtype == data::MonsterType::PENDULUM) {
        if (deckType != data::DeckType::EXTRA) {
            // not allowed
            return DeckError::FORBIDDEN;
        }
    } else {
        if (deckType == data::DeckType::EXTRA) {
            // only allows these sort of cards
            return DeckError::FORBIDDEN;
        }
    }
    return findDeck(deckType).addCard(name);
}

data::CardMap DeckSet::cards() const
{
    data::CardMap ret;
    for (auto&& kv : mDeckMap) {
        ret.emplace(data::CardMap::value_type{kv.first,kv.second.cards()});
    }
    return ret;
}

void DeckSet::deleteCard(data::DeckType deckType, const std::string& name)
{
    findDeck(deckType).deleteCard(name);
}

Deck& DeckSet::findDeck(data::DeckType deckType)
{
    return mDeckMap.find(deckType)->second;
}

const Deck& DeckSet::findDeck(data::DeckType deckType) const
{
    return mDeckMap.find(deckType)->second;
}

void DeckSet::remove()
{
    // remove all the decks and this deck set
    auto mid = findDeck(data::DeckType::MAIN).id();
    auto sid = findDeck(data::DeckType::SIDE).id();
    auto eid = findDeck(data::DeckType::EXTRA).id();

    db::SQLite3 db(DBPATH);
    db.del("deck",db::DBPair("deck_id",mid));
    db.del("deck",db::DBPair("deck_id",sid));
    db.del("deck",db::DBPair("deck_id",eid));
    db.del("deck_set",db::DBPair("deck_set_id",mID));
}

bool DeckSet::validate() const
{
    // check if the deck as built is valid and meets size requirements
    if ((findDeck(data::DeckType::MAIN).size() <
                data::DeckMin(data::DeckType::MAIN) ||
         (findDeck(data::DeckType::MAIN).size() >
          data::DeckMax(data::DeckType::MAIN)))) {
        return false;
    }

    if ((findDeck(data::DeckType::EXTRA).size() <
                data::DeckMin(data::DeckType::EXTRA) ||
         (findDeck(data::DeckType::EXTRA).size() >
          data::DeckMax(data::DeckType::EXTRA)))) {
        return false;
    }

    if ((findDeck(data::DeckType::SIDE).size() <
                data::DeckMin(data::DeckType::SIDE) ||
         (findDeck(data::DeckType::SIDE).size() >
          data::DeckMax(data::DeckType::SIDE)))) {
        return false;
    }

    return true;
}

}
}
