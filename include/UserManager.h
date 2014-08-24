#ifndef YGO_USERMANAGER_H
#define YGO_USERMANAGER_H

#include "DBUser.h"
#include "DBDeckSet.h"

namespace ygo
{


class UserManager
{
public:
    UserManager();

    DBUser login(const std::string& name);
    std::vector<DBDeckSet> deckSets(const DBUser& user);
};

}

#endif
