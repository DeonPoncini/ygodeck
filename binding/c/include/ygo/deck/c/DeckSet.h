#ifndef C_BINDING_DECKSET_H
#define C_BINDING_DECKSET_H

#include <ygo/deck/c/DataTypes.h>

#include "Format.h"
#include "User.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ygo_deck_DeckSet ygo_deck_DeckSet;

struct ygo_deck_CardMap
{
    ygo_data_StaticCardData** main;
    ygo_data_StaticCardData** side;
    ygo_data_StaticCardData** extra;
    int main_count;
    int side_count;
    int extra_count;
};
typedef struct ygo_deck_CardMap ygo_deck_CardMap;

#define DECKSET_NAME(_name) ygo_deck_DeckSet_##_name
#define DECKSET_THIS ygo_deck_DeckSet*

DECKSET_THIS DECKSET_NAME(new)(const char* name, USER_THIS u, FORMAT_THIS f);
DECKSET_THIS DECKSET_NAME(create)(const char* name, USER_THIS u, FORMAT_THIS f);

void DECKSET_NAME(delete)(DECKSET_THIS p);

char* DECKSET_NAME(name)(DECKSET_THIS p);
void DECKSET_NAME(delete_name)(char* name);
FORMAT_THIS DECKSET_NAME(format)(DECKSET_THIS p);
void DECKSET_NAME(delete_format)(FORMAT_THIS f);
ygo_deck_DeckError
DECKSET_NAME(addCard)(DECKSET_THIS p,
        ygo_data_DeckType deckType, const char* name);
void
DECKSET_NAME(deleteCard)(DECKSET_THIS p,
        ygo_data_DeckType deckType, const char* name);
ygo_deck_CardMap DECKSET_NAME(cards)(DECKSET_THIS p);
void DECKSET_NAME(delete_cards)(ygo_deck_CardMap c);
void DECKSET_NAME(remove)(DECKSET_THIS p);
int DECKSET_NAME(validate)(DECKSET_THIS p);

#ifdef __cplusplus
}
#endif

#endif
