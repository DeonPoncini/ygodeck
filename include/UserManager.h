#ifndef YGO_USERMANAGER_H
#define YGO_USERMANAGER_H

#include "DB.h"
#include "DBUser.h"

namespace ygo
{


class UserManager
{
public:
    UserManager();

    DBUser login(const std::string& name);

private:
    DB mDB;
};

}

#endif
