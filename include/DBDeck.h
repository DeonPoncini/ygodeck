#ifndef YGO_DBDECK_H
#define YGO_DBDECK_H

#include "CardData.h"
#include "FormatData.h"

#include <vector>

namespace ygo
{

class DBDeck
{
public:
    enum class DeckError
    {
        OK,
        FORBIDDEN,
        LIMIT_REACHED,
        DECK_FULL
    };

    explicit DBDeck(DeckType deckType);
    DBDeck(DeckType deckType, std::string id);

    inline DeckType deckType() const { return mDeckType; }
    inline std::string id() const { return mID; }

    DeckError addCard(const std::string& name);
    std::vector<StaticCardData> cards();
    void deleteCard(const std::string& name);

private:
    DeckType mDeckType;
    std::string mID;
    unsigned mCardCount;
};

}

#endif
