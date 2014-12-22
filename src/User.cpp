#include <ygo/deck/User.h>

#include <ygo/deck/Common.h>
#include <ygo/deck/DeckSet.h>

#include <mindbw/SQLite3.h>
#include <ygo/data/Serialize.h>

#include <stdexcept>

namespace ygo
{
namespace deck
{

User::User(std::string name, bool create) :
    mName(std::move(name))
{
    // check if the user already exists
    auto exists = false;
    std::string userid;
    mindbw::SQLite3 db(DBPATH);
    db.select(mindbw::KeyList({"name","user_id"}),"users",
            mindbw::Equal("name",mName),
            [&](mindbw::DataMap data) {
                exists = true;
                userid = data["user_id"];
            });

    if (exists) {
        mID = userid;
    } else if (create) {
        mID = db.insert("users",mindbw::ValueList({mName}));
    } else {
        throw std::runtime_error("User " + mName + " does not exist");
    }
}

std::vector<DeckSet> User::deckSets() const
{
    // return all deck sets for a given user
    std::vector<std::string> deckids;
    mindbw::SQLite3 db(DBPATH);
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
    mindbw::SQLite3 db(DBPATH);
    db.del("users",mindbw::Equal("user_id",mID));
}

}
}
