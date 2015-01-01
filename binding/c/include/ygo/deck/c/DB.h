#ifndef C_BINDING_DB_H
#define C_BINDING_DB_H

#ifdef __cplusplus
extern "C" {
#endif

#define DB_NAME(_name) ygo_deck_DB_##_name

void DB_NAME(set_path)(const char* db);
char* DB_NAME(get_path)(void);
void DB_NAME(get_path_delete)(char* path);

#ifdef __cplusplus
}
#endif

#endif
