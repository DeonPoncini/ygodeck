#ifndef C_BINDING_DB_H
#define C_BINDING_DB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ygo_deck_DB ygo_deck_DB;

#define DB_NAME(_name) ygo_deck_DB_##_name
#define DB_THIS ygo_deck_DB*

DB_THIS DB_NAME(get)(void);

void DB_NAME(set_path)(DB_THIS p, const char* db);
char* DB_NAME(get_path)(DB_THIS p);
void DB_NAME(get_path_delete)(char* path);

#ifdef __cplusplus
}
#endif

#endif
