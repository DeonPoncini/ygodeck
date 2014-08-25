#include "DBUser.h"

#include "DB.h"
#include "DBCommon.h"
#include "DBDeckSet.h"
#include "Serialize.h"

#include <stdexcept>

namespace ygo
{

DBUser::DBUser(std::string name, bool create) :
    mName(std::move(name))
{
    // check if the user already exists
    auto exists = false;
    std::string userid;
    DB db(DBPATH);
    db.select(DBKeyList({"name","user_id"}),"users",
            DBPair("name",mName),
            [&](DB::DataMap data)
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
        mID = db.insert("users",DBList({mName}));
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
    DB db(DBPATH);
    db.select("deck_set_id","user_to_decks",
            DBPair("user_id",id()),
            [&](DB::DataMap data)
            {
                deckids.push_back(data["deck_set_id"]);
            });

    // look up the deck set ids and convert them into objects
    std::vector<DBDeckSet> ret;
    for (auto&& i : deckids)
    {
        db.select(DBAll(),"deck_set",
                DBPair("deck_set_id",i),
                [&](DB::DataMap data)
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

}
