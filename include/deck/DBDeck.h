#ifndef YGO_DBDECK_H
#define YGO_DBDECK_H

#include <data/CardData.h>
#include <data/FormatData.h>

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
    inline unsigned size() const { return cards().size(); }

    DeckError addCard(const std::string& name);
    std::vector<StaticCardData> cards() const;
    void deleteCard(const std::string& name);

private:
    DeckType mDeckType;
    std::string mID;
};

}

#endif
