#ifndef C_BINDING_CARD_SELECTOR_H
#define C_BINDING_CARD_SELECTOR_H

#include <mindbw/c/DataTypes.h>
#include <ygo/data/c/DataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ygo_deck_CardSelector ygo_deck_CardSelector;

#define CARD_SEL(_name) ygo_deck_CardSelector_##_name
#define CARD_SEL_THIS ygo_deck_CardSelector*

CARD_SEL_THIS CARD_SEL(new)(void);
void CARD_SEL(delete)(CARD_SEL_THIS p);

ygo_data_StaticCardData*
CARD_SEL(query)(CARD_SEL_THIS p, const char* name);

void CARD_SEL(query_delete)(ygo_data_StaticCardData* p);

char** CARD_SEL(execute)(CARD_SEL_THIS p, int* items);
void CARD_SEL(execute_delete) (char** e, int items);

CARD_SEL_THIS CARD_SEL(name)(CARD_SEL_THIS p, const char* like);
CARD_SEL_THIS CARD_SEL(cardType)(CARD_SEL_THIS p, ygo_data_CardType ct);
CARD_SEL_THIS CARD_SEL(attribute)(CARD_SEL_THIS p, ygo_data_Attribute a);
CARD_SEL_THIS CARD_SEL(monsterType)(CARD_SEL_THIS p, ygo_data_MonsterType mt);
CARD_SEL_THIS CARD_SEL(type)(CARD_SEL_THIS p, ygo_data_Type t);
CARD_SEL_THIS CARD_SEL(level)(CARD_SEL_THIS p, int l, mindbw_Operator op);
CARD_SEL_THIS CARD_SEL(attack)(CARD_SEL_THIS p, int a, mindbw_Operator op);
CARD_SEL_THIS CARD_SEL(defense)(CARD_SEL_THIS p, int d, mindbw_Operator op);
CARD_SEL_THIS CARD_SEL(lpendulum)(CARD_SEL_THIS p, int l, mindbw_Operator op);
CARD_SEL_THIS CARD_SEL(rpendulum)(CARD_SEL_THIS p, int r, mindbw_Operator op);
CARD_SEL_THIS CARD_SEL(spellType)(CARD_SEL_THIS p, ygo_data_SpellType st);
CARD_SEL_THIS CARD_SEL(trapType)(CARD_SEL_THIS p, ygo_data_TrapType tt);


#ifdef __cplusplus
}
#endif

#endif
