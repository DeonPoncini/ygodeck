#ifndef YGO_USER_H
#define YGO_USER_H

#include <string>
#include <vector>

namespace ygo
{
namespace deck
{

class DeckSet;

class User
{
public:
    User(std::string name, bool create = false);

    std::string name() const { return mName; }
    std::string id() const { return mID; }

    std::vector<DeckSet> deckSets() const;
    void remove();

private:
    std::string mName;
    std::string mID;
};

}
}

#endif
