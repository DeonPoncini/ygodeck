#ifndef YGO_DECKBUILDER_H
#define YGO_DECKBUILDER_H

#include <string>
#include <vector>

#include "DB.h"
#include "FormatData.h"

namespace ygo
{

class DeckBuilder
{
public:
    typedef std::vector<std::string> FormatType;

    enum class DeckError
    {
        OK,
        FORBIDDEN,
        LIMIT_REACHED,
        DECK_FULL
    };

    enum class DeckCreateError
    {
        OK,
        ALREADY_EXISTS,
        INVALID_FORMAT
    };

    enum class DeckOpenError
    {
        OK,
        DOES_NOT_EXIST
    };

    DeckBuilder();

    DeckCreateError create(const std::string& name, Format format,
            const std::string& formatDate);

    DeckOpenError open(const std::string& name);

    DeckError addCard(DeckType deck, const std::string& name);
    std::vector<std::string> getCards(DeckType deck);
    void deleteCard(DeckType deck, const std::string& name);

    FormatType formats() const;
    FormatType formatDates();

    inline Format format() const { return mFormat; }
    inline std::string formatDate() const { return mFormatDate; }

private:
    DB mDB;

    std::string mMainID;
    std::string mSideID;
    std::string mExtraID;

    std::string mName;
    std::string mFormatDate;
    Format mFormat;

    std::string deckID(DeckType deckType);
    void createDeck(DeckType deckType);
    bool exists();
    bool formatExists();
    std::string cardID(const std::string& name);
    int cardCount(const std::string& name);
    int cardCheck(const std::string& cardid, const std::string& deckid);

    std::string deckTypeToId(DeckType deckType) const;
    int limitation(Limitation limitation) const;

};

}

#endif
