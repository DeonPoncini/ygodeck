#ifndef YGO_DBUSER_H
#define YGO_DBUSER_H

#include <string>

namespace ygo
{

class DBUser
{
public:
    DBUser(std::string name, std::string id);

    std::string name() const { return mName; }
    std::string id() const { return mID; }

private:
    std::string mName;
    std::string mID;
};

}

#endif
