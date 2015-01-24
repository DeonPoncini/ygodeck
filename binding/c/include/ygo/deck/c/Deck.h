#ifndef C_BINDING_DECK_H
#define C_BINDING_DECK_H

#include <ygo/data/c/DataTypes.h>
#include <ygo/deck/c/DataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ygo_deck_Deck ygo_deck_Deck;

#define DECK_NAME(_name) ygo_deck_Deck_##_name
#define DECK_THIS ygo_deck_Deck*

DECK_THIS DECK_NAME(new) (ygo_data_DeckType deckType);
DECK_THIS DECK_NAME(new_id)
(ygo_data_DeckType deckType, const char* id);
void DECK_NAME(delete) (DECK_THIS p);

ygo_data_DeckType DECK_NAME(deckType)(DECK_THIS p);
char* DECK_NAME(id)(DECK_THIS p);
void DECK_NAME(delete_id)(char* id);
unsigned DECK_NAME(size)(DECK_THIS p);

ygo_deck_DeckError DECK_NAME(addCard)(DECK_THIS p, const char* name);
ygo_data_StaticCardData** DECK_NAME(cards)(DECK_THIS p, int* count);
void DECK_NAME(delete_cards)(ygo_data_StaticCardData** s, int count);
void DECK_NAME(deleteCard)(DECK_THIS p, const char* name);
void DECK_NAME(remove)(DECK_THIS p);

#ifdef __cplusplus
}
#endif


#endif
