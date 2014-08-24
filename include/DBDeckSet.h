#ifndef YGO_DBDECKSET_H
#define YGO_DBDECKSET_H

#include <string>
#include <vector>

#include "DBDeck.h"
#include "DBFormat.h"
#include "DBUser.h"
#include "FormatData.h"

namespace ygo
{

typedef std::map<DeckType,DBDeck> DeckMap;
typedef std::map<DeckType,std::vector<StaticCardData>> CardMap;

class DBDeckSet
{
public:
    DBDeckSet(const std::string& name, const DBUser& user,
            const DBFormat& format, bool create = false);

    DBDeck::DeckError addCard(DeckType deckType, const std::string& name);
    CardMap cards();
    void deleteCard(DeckType deckType, const std::string& name);

private:
    DeckMap mDeckMap;

    std::string mName;
    DBUser mUser;
    DBFormat mFormat;
    std::string mID;

    bool exists();

    void create();
    void open();

    DBDeck& findDeck(DeckType deckType);
};

}

#endif
