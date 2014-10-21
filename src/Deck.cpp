#include "Deck.h"
#include "Common.h"

#include <db/SQLite3.h>
#include <ygo/data/Serialize.h>

namespace ygo
{
namespace deck
{

Deck::Deck(data::DeckType deckType) :
    mDeckType(deckType)
{
    db::SQLite3 db(DBPATH);
    mID = db.insert("deck",
            db::DBList({fromDeckType(mDeckType)}));
}

Deck::Deck(data::DeckType deckType, std::string id) :
    mDeckType(deckType),
    mID(id)
{
}

DeckError Deck::addCard(const std::string& name)
{
    if (cards().size() >= DeckMax(mDeckType))
    {
        return DeckError::DECK_FULL;
    }

    // get the card ID
    db::SQLite3 db(DBPATH);
    std::string id;
    db.select("card_id","card",
            db::DBPair("name",name),
            [&](db::SQLite3::DataMap data)
            {
                id = data["card_id"];
            });

    // insert into the deck
    db.insert("deck_to_cards",
            db::DBList({
                mID,
                id
                }));

    return DeckError::OK;
}

std::vector<data::StaticCardData> Deck::cards() const
{
    // search all the cards part of this deck
    db::SQLite3 db(DBPATH);
    std::vector<std::string> ids;
    db.select("card_id","deck_to_cards",
            db::DBPair("deck_id",mID),
            [&](db::SQLite3::DataMap data)
            {
                ids.push_back(data["card_id"]);
            });

    // get all the card info
    std::vector<data::StaticCardData> ret;
    for (auto&& i : ids)
    {
        db.select(db::DBAll(),"card",db::DBPair("card_id",i),
                [&](db::SQLite3::DataMap data)
                {
                    data::StaticCardData s;
                    s.name = data["name"].c_str();
                    s.cardType = data::toCardType(data["cardType"]);
                    s.attribute = data::toAttribute(data["attribute"]);
                    s.monsterType = data::toMonsterType(data["monsterType"]);
                    s.type = data::toType(data["type"]);
                    s.monsterAbility = data::toMonsterType(data["monsterAbility"]);
                    s.level = std::atoi(data["level"].c_str());
                    s.attack = std::atoi(data["attack"].c_str());
                    s.defense = std::atoi(data["defense"].c_str());
                    s.lpendulum = std::atoi(data["lpendulum"].c_str());
                    s.rpendulum = std::atoi(data["rpendulum"].c_str());
                    s.spellType = data::toSpellType(data["spellType"]);
                    s.trapType = data::toTrapType(data["trapType"]);
                    ret.push_back(s);
                });
    }
    return ret;
}

void Deck::deleteCard(const std::string& name)
{
    // lookup this card id
    std::string cardid;
    db::SQLite3 db(DBPATH);
    db.select("card_id","card",db::DBPair("name",name),
            [&](db::SQLite3::DataMap data)
            {
                cardid = data["card_id"];
            });

    // return every relation for this deck where this card id matches
    std::vector<std::string> ids;
    db.select("relation_id","deck_to_cards",
            db::DBAnd({db::DBPair("deck_id",mID),db::DBPair("card_id",cardid)}),
            [&](db::SQLite3::DataMap data)
            {
                ids.push_back(data["relation_id"]);
            });

    // delete only one of these relations
    if (!ids.empty())
    {
        db.del("deck_to_cards",db::DBPair("relation_id",ids[0]));
    }
}

}
}
