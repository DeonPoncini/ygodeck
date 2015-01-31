#ifndef YGO_DECK_DECKSET_H
#define YGO_DECK_DECKSET_H

#include <string>
#include <vector>

#include "Deck.h"
#include "Format.h"
#include "User.h"

#include <ygo/data/FormatData.h>

namespace ygo
{
namespace deck
{

typedef std::map<data::DeckType,Deck> DeckMap;

class DeckSet
{
public:
    DeckSet(const std::string& name, const User& user,
            const Format& format, bool create = false);

    inline std::string name() const { return mName; }
    inline Format format() const { return mFormat; }

    DeckError addCard(data::DeckType deckType, const std::string& name);
    data::CardMap cards() const;
    void deleteCard(data::DeckType deckType, const std::string& name);
    void remove();
    bool validate() const;
    std::string exportCards() const;

private:
    DeckMap mDeckMap;

    std::string mName;
    User mUser;
    Format mFormat;
    std::string mID;

    bool exists();

    void create();
    void open();

    Deck& findDeck(data::DeckType deckType);
    const Deck& findDeck(data::DeckType deckType) const;
};

}
}

#endif
