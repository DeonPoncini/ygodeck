#include "UserManager.h"

#include "DB.h"
#include "DBCommon.h"
#include "Serialize.h"

namespace ygo
{

UserManager::UserManager()
{
}

DBUser UserManager::login(const std::string& name)
{
    // check if the user already exists
    auto exists = false;
    std::string userid;
    DB db(DBPATH);
    db.select(DBKeyList({"name","user_id"}),"users",
            DBPair("name",name),
            [&](DB::DataMap data)
            {
                exists = true;
                userid = data["user_id"];
            });

    if (exists)
    {
        return {name,userid};
    }

    // create a new user
    userid = db.insert("users",DBList({name}));
    return {name,userid};
}

std::vector<DBDeckSet> UserManager::deckSets(const DBUser& user)
{
    // return all deck sets for a given user
    std::vector<std::string> deckids;
    DB db(DBPATH);
    db.select("deck_set_id","user_to_decks",
            DBPair("user_id",user.id()),
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
                    ret.emplace_back(DBDeckSet{data["name"],user,f});
                });
    }
    return ret;
}

}
