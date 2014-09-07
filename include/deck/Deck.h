#ifndef YGO_DECK_DECK_H
#define YGO_DECK_DECK_H

#include "DeckError.h"

#include <data/CardData.h>
#include <data/FormatData.h>

#include <vector>

namespace ygo
{
namespace deck
{

class Deck
{
public:
    explicit Deck(data::DeckType deckType);
    Deck(data::DeckType deckType, std::string id);

    inline data::DeckType deckType() const { return mDeckType; }
    inline std::string id() const { return mID; }
    inline unsigned size() const { return cards().size(); }

    DeckError addCard(const std::string& name);
    std::vector<data::StaticCardData> cards() const;
    void deleteCard(const std::string& name);

private:
    data::DeckType mDeckType;
    std::string mID;
};

}
}

#endif
