#include <ygo/deck/User.h>

#include <ygo/deck/DB.h>
#include <ygo/deck/DeckSet.h>

#include <kizhi/Log.h>
#include <mindbw/SQLite3.h>
#include <ygo/data/Serialize.h>

#include <cassert>
#include <stdexcept>

namespace ygo
{
namespace deck
{

User::User(std::string name, bool create) :
    mName(std::move(name))
{
    KIZHI_TRACE_F << "User " << mName << " create " << create;
    // check if the user already exists
    auto exists = false;
    std::string userid;
    mindbw::SQLite3 db(DB::get().path());
    db.select(mindbw::KeyList({"name","user_id"}),"users",
            mindbw::Equal("name",mName),
            [&](mindbw::DataMap data) {
                exists = true;
                userid = data["user_id"];
            });

    if (exists) {
        mID = userid;
        KIZHI_TRACE_F << "User " << mName << " exists " << mID;
    } else if (create) {
        mID = db.insert("users",mindbw::ValueList({mName}));
        KIZHI_TRACE_F << "User " << mName << " created " << mID;
    } else {
        KIZHI_FATAL_F << "User " << mName << " does not exist ";
        throw std::runtime_error("User " + mName + " does not exist");
    }
}

std::vector<DeckSet> User::deckSets() const
{
    // return all deck sets for a given user
    std::vector<std::string> deckids;
    assert(!DB::get().path().empty());
    mindbw::SQLite3 db(DB::get().path());
    db.select("deck_set_id","user_to_decks",
            mindbw::Equal("user_id",id()),
            [&](mindbw::DataMap data) {
                deckids.push_back(data["deck_set_id"]);
            });

    // look up the deck set ids and convert them into objects
    std::vector<DeckSet> ret;
    for (auto&& i : deckids) {
        db.select(mindbw::All(),"deck_set",
                mindbw::Equal("deck_set_id",i),
                [&](mindbw::DataMap data) {
                    // extract the format
                    Format f(data::toFormat(data["format"]),
                        data["format_date"]);

                    // add a new DBDeckSet
                    ret.emplace_back(DeckSet{data["name"],*this,f});
                });
    }
    KIZHI_FATAL_F << "User " << mName << " has " << ret.size() << " deckSets";
    return ret;
}

void User::remove()
{
    // delete all the deck sets associated with this user
    auto sets = deckSets();
    for (auto&& s : sets) {
        s.remove();
    }

    // delete the user
    mindbw::SQLite3 db(DB::get().path());
    db.del("users",mindbw::Equal("user_id",mID));
    // remove all links between user and decks
    db.del("user_to_decks", mindbw::Equal("user_id", mID));
    KIZHI_FATAL_F << "User " << mName << " has been removed";
}

}
}
