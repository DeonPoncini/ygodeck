#ifndef C_BINDING_FORMAT_H
#define C_BINDING_FORMAT_H

#include <ygo/data/c/DataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ygo_deck_Format ygo_deck_Format;

#define FORMAT_NAME(_name) ygo_deck_Format_##_name
#define FORMAT_THIS ygo_deck_Format*

FORMAT_THIS FORMAT_NAME(new)(ygo_data_Format format, const char* date);
void FORMAT_NAME(delete)(FORMAT_THIS p);

ygo_data_Format FORMAT_NAME(format)(FORMAT_THIS p);
char* FORMAT_NAME(formatDate)(FORMAT_THIS p);
void FORMAT_NAME(delete_formatDate)(char* d);
int FORMAT_NAME(cardCount)(FORMAT_THIS p, const char* card);

char** FORMAT_NAME(formatDates)(FORMAT_THIS p, int* count);
void FORMAT_NAME(delete_formatDates)(char** d, int count);
char** FORMAT_NAME(formats)(FORMAT_THIS p, int* count);
void FORMAT_NAME(delete_formats)(char** d, int count);

#ifdef __cplusplus
}
#endif

#endif
