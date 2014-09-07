#include "DBUser.h"

#include "DBCommon.h"
#include "DBDeckSet.h"

#include <db/SQLite3.h>
#include <data/Serialize.h>

#include <stdexcept>

namespace ygo
{

DBUser::DBUser(std::string name, bool create) :
    mName(std::move(name))
{
    // check if the user already exists
    auto exists = false;
    std::string userid;
    db::SQLite3 db(DBPATH);
    db.select(db::DBKeyList({"name","user_id"}),"users",
            db::DBPair("name",mName),
            [&](db::SQLite3::DataMap data)
            {
                exists = true;
                userid = data["user_id"];
            });

    if (exists)
    {
        mID = userid;
    }
    else if (create)
    {
        mID = db.insert("users",db::DBList({mName}));
    }
    else
    {
        throw std::runtime_error("User " + mName + " does not exist");
    }
}

std::vector<DBDeckSet> DBUser::deckSets() const
{
    // return all deck sets for a given user
    std::vector<std::string> deckids;
    db::SQLite3 db(DBPATH);
    db.select("deck_set_id","user_to_decks",
            db::DBPair("user_id",id()),
            [&](db::SQLite3::DataMap data)
            {
                deckids.push_back(data["deck_set_id"]);
            });

    // look up the deck set ids and convert them into objects
    std::vector<DBDeckSet> ret;
    for (auto&& i : deckids)
    {
        db.select(db::DBAll(),"deck_set",
                db::DBPair("deck_set_id",i),
                [&](db::SQLite3::DataMap data)
                {
                    // extract the format
                    DBFormat f(toFormat(data["format"]),
                        data["format_date"]);

                    // add a new DBDeckSet
                    ret.emplace_back(DBDeckSet{data["name"],*this,f});
                });
    }
    return ret;
}

void DBUser::remove()
{
    // delete all the deck sets associated with this user
    auto sets = deckSets();
    for (auto&& s : sets)
    {
        s.remove();
    }

    // delete the user
    db::SQLite3 db(DBPATH);
    db.del("users",db::DBPair("user_id",mID));
}

}
