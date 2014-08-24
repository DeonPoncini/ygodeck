#ifndef YGO_DBUSER_H
#define YGO_DBUSER_H

#include <string>
#include <vector>

namespace ygo
{

class DBDeckSet;

class DBUser
{
public:
    DBUser(std::string name, bool create = false);

    std::string name() const { return mName; }
    std::string id() const { return mID; }

    std::vector<DBDeckSet> deckSets(const DBUser& user);

private:
    std::string mName;
    std::string mID;
};

}

#endif
