#include <ygo/deck/DeckSet.h>

#include <ygo/deck/DB.h>

#include <kizhi/Log.h>
#include <mindbw/SQLite3.h>
#include <ygo/data/Serialize.h>

#include <stdexcept>
#include <vector>
#include <unordered_map>

namespace ygo
{
namespace deck
{

static const std::unordered_map<std::string, std::vector<std::string>>
nameConditions = {
    {"A Legendary Ocean",
        {"Lemuria, the Forgotten City", "Umi"}},
    {"Lemuria, the Forgotten City",
        {"A Legendary Ocean", "Umi"}},
    {"Umi",
        {"A Legendary Ocean", "Lemuria, the Forgotten City"}},
    {"Cyber Harpie Lady",
        {"Harpie Lady", "Harpie Lady 1", "Harpie Lady 2", "Harpie Lady 3"}},
    {"Harpie Lady",
        {"Cyber Harpie Lady", "Harpie Lady 1", "Harpie Lady 2", "Harpie Lady 3"}},
    {"Harpie Lady 1",
        {"Cyber Harpie Lady", "Harpie Lady", "Harpie Lady 2", "Harpie Lady 3"}},
    {"Harpie Lady 2",
        {"Cyber Harpie Lady", "Harpie Lady", "Harpie Lady 1", "Harpie Lady 3"}},
    {"Harpie Lady 3",
        {"Cyber Harpie Lady", "Harpie Lady", "Harpie Lady 1", "Harpie Lady 2"}},
    {"Neo-Spacian Aqua Dolphin",
        {"Neo-Spacian Marine Dolphin"}},
    {"Neo-Spacian Marine Dolphin",
        {"Neo-Spacian Aqua Dolphin"}},
    {"The Eye of Timaeus",
        {"Legendary Dragon Timaeus"}},
    {"Legendary Dragon Timaeus",
        {"The Eye of Timaeus"}},
    {"Fusion Substitute",
        {"Polymerization"}},
    {"Polymerization",
        {"Fusion Substitute"}},
    {"Neo-Spacian Glow Moss",
        {"Neo-Spacian Twinkle Moss"}},
    {"Neo-Spacian Twinkle Moss",
        {"Neo-Spacian Glow Moss"}},
};

std::string cardID(const std::string& name)
{
    std::string id;
    mindbw::SQLite3 db(DB::get().path());
    db.select("card_id","card",
            mindbw::Equal("name",name),
            [&](mindbw::DataMap data) {
                id = data["card_id"];
            });
    KIZHI_TRACE_F << "Card " << name << " has ID " << id;
    return id;
}

int cardCheck(const std::string& cardid, const std::string& deckid)
{
    auto count = 0;
    mindbw::SQLite3 db(DB::get().path());
    db.select("relation_id","deck_to_cards",
            mindbw::And({
                mindbw::Equal("deck_id",deckid),
                mindbw::Equal("card_id",cardid)}),
            [&](mindbw::DataMap data) {
                count += data.size();
            });
    KIZHI_TRACE_F << "Card ID " << cardid << " is present "
        << count << " times";
    return count;
}

data::MonsterType monsterType(const std::string& name)
{
    std::string mtype;
    mindbw::SQLite3 db(DB::get().path());
    db.select("monsterType","card",
            mindbw::Equal("name",name),
            [&](mindbw::DataMap data) {
                mtype = data["monsterType"];
            });
    KIZHI_TRACE_F << "Looked up monster type " << mtype;
    return data::toMonsterType(mtype);
}

DeckSet::DeckSet(const std::string& name, const User& user,
        const Format& format, bool create) :
    mName(name),
    mUser(user),
    mFormat(format)
{
    KIZHI_TRACE_F << "DeckSet " << mName << " User: " << mUser.name()
        << " Format: " << mFormat.formatDate() << ","
        << data::fromFormat(mFormat.format()) << " create: " << create;
    if (exists()) {
        KIZHI_TRACE_F << "DeckSet " << mName << " already exists";
        open();
    } else if (create) {
        KIZHI_TRACE_F << "DeckSet " << mName << " is now being created";
        DeckSet::create();
    } else {
        KIZHI_FATAL_F << "DeckSet " << mName << " does not exist";
        throw std::runtime_error("Deck Set " + mName + " does not exist");
    }
}

bool DeckSet::exists()
{
    auto exists = false;
    mindbw::SQLite3 db(DB::get().path());
    // check if the deck named has the same format
    db.select(mindbw::KeyList({"deck_set_id","format","format_date"}),"deck_set",
            mindbw::Equal("name",mName),
            [&](mindbw::DataMap data) {
                if (data["format"] == fromFormat(mFormat.format())
                    && data["format_date"] == mFormat.formatDate()) {
                    exists = true;
                    mID = data["deck_set_id"];
                    KIZHI_TRACE_F << "Found " << mName << " with ID " << mID;
                }
            });
    // now check that deck belongs to us
    if (exists) {
        auto owned = false;
        db.select("user_id","user_to_decks",
                mindbw::Equal("deck_set_id",mID),
                [&](mindbw::DataMap data) {
                    if (data["user_id"] == mUser.id()) {
                        owned = true;
                    }
                });
        if (owned) {
            KIZHI_TRACE_F << mName << " belongs to  " << mUser.name()
                << " which is us";
            return true;
        }
    }
    KIZHI_TRACE_F << mName << " is not our deckSet";
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
    mindbw::SQLite3 db(DB::get().path());
    mID = db.insert("deck_set",
            mindbw::ValueList({
                mName,
                fromFormat(mFormat.format()),
                mFormat.formatDate(),
                findDeck(data::DeckType::MAIN).id(),
                findDeck(data::DeckType::SIDE).id(),
                findDeck(data::DeckType::EXTRA).id(),
                }));

    // map the deck to the user
    db.insert("user_to_decks",
            mindbw::ValueList({mUser.id(),mID}));
    KIZHI_TRACE_F << "Created deckSet " << mName;
}

void DeckSet::open()
{
    // find out the deck ids from the main id, note it is set in exists
    mindbw::SQLite3 db(DB::get().path());
    db.select(mindbw::KeyList({"main_deck_id","side_deck_id","extra_deck_id"}),
            "deck_set",mindbw::Equal("deck_set_id",mID),
            [&](mindbw::DataMap data) {
                mDeckMap.emplace(DeckMap::value_type{data::DeckType::MAIN,
                        Deck{data::DeckType::MAIN, data["main_deck_id"]}});
                mDeckMap.emplace(DeckMap::value_type{data::DeckType::SIDE,
                        Deck{data::DeckType::SIDE, data["side_deck_id"]}});
                mDeckMap.emplace(DeckMap::value_type{data::DeckType::EXTRA,
                        Deck{data::DeckType::EXTRA, data["extra_deck_id"]}});
            });
    KIZHI_TRACE_F << "Opened deckSet " << mName;
}

DeckError DeckSet::addCard(data::DeckType deckType,
        const std::string& name)
{
    KIZHI_TRACE_F << "Adding card " << name
        << " to deck " << data::fromDeckType(deckType) << " in deckSet "
        << mName;
    // check if we can add to any card list
    auto count = mFormat.cardCount(name);
    KIZHI_TRACE_F << name << " is allowed " << count << " times in a deck";
    auto exist = 0;
    // check the existance of any synonyms
    auto it = nameConditions.find(name);
    std::vector<std::string> synonyms = {name};
    if (it != nameConditions.end()) {
        synonyms.insert(std::end(synonyms),
                std::begin(it->second), std::end(it->second));
    }
    for (auto&& n : synonyms) {
        auto cardid = cardID(n);
        for (auto&& kv : mDeckMap) {
            exist += cardCheck(cardid,kv.second.id());
        }
    }
    KIZHI_TRACE_F << name << " exists " << exist << " times in our decks";

    if (count <= exist) {
        KIZHI_TRACE_F << name << " has reached a limit in our deck";
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
            KIZHI_TRACE_F << name << " is forbidden from this deck type";
            return DeckError::FORBIDDEN;
        }
    } else {
        if (deckType == data::DeckType::EXTRA) {
            // only allows these sort of cards
            KIZHI_TRACE_F << name << " is forbidden from this deck type";
            return DeckError::FORBIDDEN;
        }
    }
    KIZHI_TRACE_F << "Successfully added " << name;
    return findDeck(deckType).addCard(name);
}

data::CardMap DeckSet::cards() const
{
    data::CardMap ret;
    for (auto&& kv : mDeckMap) {
        ret.emplace(data::CardMap::value_type{kv.first,kv.second.cards()});
    }
    KIZHI_TRACE_F << "Returning card map";
    return ret;
}

void DeckSet::deleteCard(data::DeckType deckType, const std::string& name)
{
    KIZHI_TRACE_F << "Deleting card " << name << " from deck " <<
        data::fromDeckType(deckType) << " in deckSet " << mName;
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
    mindbw::SQLite3 db(DB::get().path());
    findDeck(data::DeckType::MAIN).remove();
    findDeck(data::DeckType::SIDE).remove();
    findDeck(data::DeckType::EXTRA).remove();
    // remove all links between user and decks
    db.del("user_to_decks", mindbw::Equal("deck_set_id", mID));
    db.del("deck_set",mindbw::Equal("deck_set_id",mID));
    KIZHI_TRACE_F << "Removed deckSet " << mName;
}

bool DeckSet::validate() const
{
    // check if the deck as built is valid and meets size requirements
    if ((findDeck(data::DeckType::MAIN).size() <
                data::deckMin(data::DeckType::MAIN) ||
         (findDeck(data::DeckType::MAIN).size() >
          data::deckMax(data::DeckType::MAIN)))) {
        KIZHI_TRACE_F << mName << " is invalid due to main deck size";
        return false;
    }

    if ((findDeck(data::DeckType::EXTRA).size() <
                data::deckMin(data::DeckType::EXTRA) ||
         (findDeck(data::DeckType::EXTRA).size() >
          data::deckMax(data::DeckType::EXTRA)))) {
        KIZHI_TRACE_F << mName << " is invalid due to extra deck size";
        return false;
    }

    if ((findDeck(data::DeckType::SIDE).size() <
                data::deckMin(data::DeckType::SIDE) ||
         (findDeck(data::DeckType::SIDE).size() >
          data::deckMax(data::DeckType::SIDE)))) {
        KIZHI_TRACE_F << mName << " is invalid due to side deck size";
        return false;
    }

    KIZHI_TRACE_F << mName << " is a valid deck set";
    return true;
}

std::string DeckSet::exportCards() const
{
    return fromCardMap(cards());
}

}
}
