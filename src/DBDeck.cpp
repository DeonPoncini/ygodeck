#include "DB.h"
#include "DBDeck.h"
#include "DBCommon.h"
#include "DeckBuilder.h"
#include "Serialize.h"

namespace ygo
{

DBDeck::DBDeck(DeckType deckType) :
    mDeckType(deckType),
    mCardCount(0)
{
    DB db(DBPATH);
    mID = db.insert("deck",
            DBList({fromDeckType(mDeckType)}));
}

DBDeck::DBDeck(DeckType deckType, std::string id) :
    mDeckType(deckType),
    mID(id)
{
    mCardCount = cards().size();
}

DBDeck::DeckError DBDeck::addCard(const std::string& name)
{
    if (mCardCount >= DeckMax(mDeckType))
    {
        return DeckError::DECK_FULL;
    }

    // get the card ID
    DB db(DBPATH);
    std::string id;
    db.select("card_id","card",
            DBPair("name",name),
            [&](DB::DataMap data)
            {
                id = data["card_id"];
            });

    // insert into the deck
    db.insert("deck_to_cards",
            DBList({
                mID,
                id
                }));

    // increase the number of cards in the deck
    mCardCount++;

    return DeckError::OK;
}

std::vector<StaticCardData> DBDeck::cards() const
{
    // search all the cards part of this deck
    DB db(DBPATH);
    std::vector<std::string> ids;
    db.select("card_id","deck_to_cards",
            DBPair("deck_id",mID),
            [&](DB::DataMap data)
            {
                ids.push_back(data["card_id"]);
            });

    // get all the card info
    std::vector<StaticCardData> ret;
    for (auto&& i : ids)
    {
        db.select(DBAll(),"card",DBPair("card_id",i),
                [&](DB::DataMap data)
                {
                    StaticCardData s;
                    s.name = data["name"];
                    s.cardType = toCardType(data["cardType"]);
                    s.attribute = toAttribute(data["attribute"]);
                    s.monsterType = toMonsterType(data["monsterType"]);
                    s.type = toType(data["type"]);
                    s.monsterAbility = toMonsterAbility(data["monsterAbility"]);
                    s.level = std::atoi(data["level"].c_str());
                    s.attack = std::atoi(data["attack"].c_str());
                    s.defense = std::atoi(data["defense"].c_str());
                    s.lpendulum = std::atoi(data["lpendulum"].c_str());
                    s.rpendulum = std::atoi(data["rpendulum"].c_str());
                    s.spellType = toSpellType(data["spellType"]);
                    s.trapType = toTrapType(data["trapType"]);
                    ret.push_back(s);
                });
    }
    return ret;
}

void DBDeck::deleteCard(const std::string& name)
{
}

}
