#ifndef C_BINDING_USER_H
#define C_BINDING_USER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ygo_deck_User ygo_deck_User;
typedef struct ygo_deck_DeckSet ygo_deck_DeckSet;

#define USER_NAME(_name) ygo_deck_User_##_name
#define USER_THIS ygo_deck_User*

USER_THIS USER_NAME(new)(const char* name);
USER_THIS USER_NAME(new_create)(const char* name);
void USER_NAME(delete)(USER_THIS p);

char* USER_NAME(name)(USER_THIS p);
void USER_NAME(delete_name)(char* name);

char* USER_NAME(id)(USER_THIS p);
void USER_NAME(delete_id)(char* id);

ygo_deck_DeckSet** USER_NAME(deckSets)(USER_THIS p, int* count);
void USER_NAME(delete_deckSets)(ygo_deck_DeckSet** d, int count);

#ifdef __cplusplus
}
#endif

#endif
