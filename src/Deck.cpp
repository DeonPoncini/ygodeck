#include <ygo/deck/Deck.h>
#include <ygo/deck/DB.h>

#include <kizhi/Log.h>
#include <mindbw/SQLite3.h>
#include <ygo/data/Serialize.h>

#include <cstdlib>

namespace ygo
{
namespace deck
{

Deck::Deck(data::DeckType deckType) :
    mDeckType(deckType)
{
    mindbw::SQLite3 db(DB::get().path());
    auto dt = fromDeckType(mDeckType);
    mID = db.insert("deck",
            mindbw::ValueList({dt}));
    KIZHI_TRACE_F << "New " << dt << " deck";
}

Deck::Deck(data::DeckType deckType, std::string id) :
    mDeckType(deckType),
    mID(id)
{
    KIZHI_TRACE_F << "Opening " << fromDeckType(mDeckType) << " deck " << mID;
}

DeckError Deck::addCard(const std::string& name)
{
    KIZHI_TRACE_F << "Adding card " << name << " to "
        << fromDeckType(mDeckType) << " deck";
    if (cards().size() >= data::deckMax(mDeckType)) {
        KIZHI_TRACE_F << "Deck is full";
        return DeckError::DECK_FULL;
    }

    // get the card ID
    mindbw::SQLite3 db(DB::get().path());
    std::string id;
    db.select("card_id","card",
            mindbw::Equal("name",name),
            [&](mindbw::DataMap data) {
                id = data["card_id"];
            });
    KIZHI_TRACE_F << "Card " << name << " id " << id;

    // insert into the deck
    db.insert("deck_to_cards",
            mindbw::ValueList({
                mID,
                id
                }));

    return DeckError::OK;
}

std::vector<data::StaticCardData> Deck::cards() const
{
    // search all the cards part of this deck
    mindbw::SQLite3 db(DB::get().path());
    std::vector<std::string> ids;
    db.select("card_id","deck_to_cards",
            mindbw::Equal("deck_id",mID),
            [&](mindbw::DataMap data) {
                ids.push_back(data["card_id"]);
            });

    // get all the card info
    std::vector<data::StaticCardData> ret;
    for (auto&& i : ids) {
        db.select(mindbw::All(),"card",mindbw::Equal("card_id",i),
                [&](mindbw::DataMap data) {
                    data::StaticCardData s;
                    s.name = data["name"];
                    s.cardType = data::toCardType(data["cardType"]);
                    s.attribute = data::toAttribute(data["attribute"]);
                    s.monsterType = data::toMonsterType(data["monsterType"]);
                    s.type = data::toType(data["type"]);
                    s.monsterAbility = data::toMonsterType(data["monsterAbility"]);
                    s.level = atoi(data["level"].c_str());
                    s.attack = atoi(data["attack"].c_str());
                    s.defense = atoi(data["defense"].c_str());
                    s.lpendulum = atoi(data["lpendulum"].c_str());
                    s.rpendulum = atoi(data["rpendulum"].c_str());
                    s.spellType = data::toSpellType(data["spellType"]);
                    s.trapType = data::toTrapType(data["trapType"]);
                    s.text = data["text"];
                    ret.push_back(s);
                });
    }
    KIZHI_TRACE_F << "Returning deck list of size " << ret.size();
    return ret;
}

void Deck::deleteCard(const std::string& name)
{
    KIZHI_TRACE_F << "Deleting card " << name;
    // lookup this card id
    std::string cardid;
    mindbw::SQLite3 db(DB::get().path());
    db.select("card_id","card",mindbw::Equal("name",name),
            [&](mindbw::DataMap data) {
                cardid = data["card_id"];
            });

    // return every relation for this deck where this card id matches
    std::vector<std::string> ids;
    db.select("relation_id","deck_to_cards",
            mindbw::And({mindbw::Equal("deck_id",mID),mindbw::Equal("card_id",cardid)}),
            [&](mindbw::DataMap data) {
                ids.push_back(data["relation_id"]);
            });

    // delete only one of these relations
    if (!ids.empty()) {
        db.del("deck_to_cards",mindbw::Equal("relation_id",ids[0]));
    } else {
        KIZHI_TRACE_F << "Card " << name << " id " << cardid << " was not in "
            << " deck " << mID;
    }
}

void Deck::remove()
{
    mindbw::SQLite3 db(DB::get().path());
    db.del("deck",mindbw::Equal("deck_id",mID));

    // delete all the deck to card mappings for this deck
    db.del("deck_to_cards", mindbw::Equal("deck_id", mID));
    KIZHI_TRACE_F << "Removed deck " << mID;
}

}
}
