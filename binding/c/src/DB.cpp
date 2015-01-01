#include <ygo/deck/DB.h>
#include <ygo/deck/c/DB.h>

#include <zephyr/cstring.h>

extern "C" {

void DB_NAME(set_path)(const char* db)
{
    ygo::deck::DB::get().path(db);
}

char* DB_NAME(get_path)(void)
{
    auto s = ygo::deck::DB::get().path();
    return zephyr::cstring::make_copy(s);
}

void DB_NAME(get_path_delete)(char* path)
{
    zephyr::cstring::delete_copy(path);
}

}
