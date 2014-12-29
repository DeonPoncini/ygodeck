#include <ygo/deck/Deck.h>
#include <c/ygo/deck/Deck.h>

#include <ygo/data/CardData.h>

#include <zephyr/cstring.h>

extern "C" {

#define C_CAST(p) reinterpret_cast<DECK_THIS>(p)
#define CPP_CAST(p) reinterpret_cast<ygo::deck::Deck*>(p)

DECK_THIS DECK_NAME(new) (ZU(ygo)ZU(data)DeckType deckType)
{
    return C_CAST(new ygo::deck::Deck(deckType));
}

DECK_THIS DECK_NAME(new_id)
(ZU(ygo)ZU(data)DeckType deckType, const char* id)
{
    return C_CAST(new ygo::deck::Deck(deckType, id));
}

void DECK_NAME(delete) (DECK_THIS p)
{
    delete CPP_CAST(p);
}

ZU(ygo)ZU(data)DeckType DECK_NAME(deckType)(DECK_THIS p)
{
    return CPP_CAST(p)->deckType();
}

char* DECK_NAME(id)(DECK_THIS p)
{
    return zephyr::cstring::make_copy(CPP_CAST(p)->id());
}

void DECK_NAME(delete_id)(char* id)
{
    zephyr::cstring::delete_copy(id);
}

unsigned DECK_NAME(size)(DECK_THIS p)
{
    return CPP_CAST(p)->size();
}

ygo_deck_DeckError DECK_NAME(addCard)(DECK_THIS p, const char* name)
{
    return static_cast<ygo_deck_DeckError>(CPP_CAST(p)->addCard(name));
}

ygo_data_C_StaticCardData** DECK_NAME(cards)(DECK_THIS p, int* count)
{
    auto cardList = CPP_CAST(p)->cards();
    *count = cardList.size();
    // allocate enough pointers for all of them
    auto ret = new ygo_data_C_StaticCardData*[*count];
    for (auto i = 0; i < *count; i++) {
        ret[i] = ygo::data::staticDataToC(cardList[i]);
    }
    return ret;
}

void DECK_NAME(delete_cards)(ygo_data_C_StaticCardData** s, int count)
{
    for (auto i = 0; i < count; i++) {
        ygo::data::deleteCStaticData(s[i]);
    }
    delete s;
}

void DECK_NAME(deleteCard)(DECK_THIS p, const char* name)
{
    CPP_CAST(p)->deleteCard(name);
}

}
