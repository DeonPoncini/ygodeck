#include <ygo/deck/DB.h>
#include <c/ygo/deck/DB.h>

#include <zephyr/cstring.h>

extern "C" {

#define C_CAST(p) reinterpret_cast<DB_THIS>(p)
#define CPP_CAST(p) reinterpret_cast<ygo::deck::DB*>(p)

DB_THIS DB_NAME(get)(void)
{
    return C_CAST(&ygo::deck::DB::get());
}

void DB_NAME(set_path)(DB_THIS p, const char* db)
{
    CPP_CAST(p)->path(db);
}

char* DB_NAME(get_path)(DB_THIS p)
{
    auto s = CPP_CAST(p)->path();
    return zephyr::cstring::make_copy(s);
}

void DB_NAME(get_path_delete)(char* path)
{
    zephyr::cstring::delete_copy(path);
}

}
