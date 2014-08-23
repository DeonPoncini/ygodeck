#include "UserManager.h"
#include "DBCommon.h"

namespace ygo
{

UserManager::UserManager() :
    mDB(DBPATH)
{
}

DBUser UserManager::login(const std::string& name)
{
    // check if the user already exists
    auto exists = false;
    std::string userid;
    mDB.select(DBKeyList({"name","user_id"}),"users",
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
    userid = mDB.insert("users",DBList({name}));
    return {name,userid};
}

}
