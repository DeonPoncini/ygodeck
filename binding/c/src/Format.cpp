#include <ygo/deck/Format.h>
#include <c/ygo/deck/Format.h>

#include <ygo/data/cpp/DataTypes.h>

#include <zephyr/cstring.h>

extern "C" {

#define C_CAST(p) reinterpret_cast<FORMAT_THIS>(p)
#define CPP_CAST(p) reinterpret_cast<ygo::deck::Format*>(p)

FORMAT_THIS FORMAT_NAME(new)(ygo_data_Format format, const char* date)
{
    return C_CAST(new ygo::deck::Format(
                static_cast<ygo::data::Format>(format), date));
}

void FORMAT_NAME(delete)(FORMAT_THIS p)
{
    delete CPP_CAST(p);
}

ygo_data_Format FORMAT_NAME(format)(FORMAT_THIS p)
{
    return static_cast<ygo_data_Format>(CPP_CAST(p)->format());
}

char* FORMAT_NAME(formatDate)(FORMAT_THIS p)
{
    return zephyr::cstring::make_copy(CPP_CAST(p)->formatDate());
}

void FORMAT_NAME(delete_formatDate)(char* d)
{
    zephyr::cstring::delete_copy(d);
}

int FORMAT_NAME(cardCount)(FORMAT_THIS p, const char* card)
{
    return CPP_CAST(p)->cardCount(card);
}

char** FORMAT_NAME(formatDates)(FORMAT_THIS p, int* count)
{
    auto dates = CPP_CAST(p)->formatDates();
    *count = dates.size();
    char** ret = new char*[*count];
    for (auto i = 0; i < *count; i++) {
        ret[i] = zephyr::cstring::make_copy(dates[i]);
    }
    return ret;
}

void FORMAT_NAME(delete_formatDates)(char** d, int count)
{
    for (auto i = 0; i < count; i++) {
        zephyr::cstring::delete_copy(d[i]);
    }
    delete[] d;
}

char** FORMAT_NAME(formats)(FORMAT_THIS p, int* count)
{
    auto formats = CPP_CAST(p)->formats();
    *count = formats.size();
    char** ret = new char*[*count];
    for (auto i = 0; i < *count; i++) {
        ret[i] = zephyr::cstring::make_copy(formats[i]);
    }
    return ret;
}

void FORMAT_NAME(delete_formats)(char** d, int count)
{
    for (auto i = 0; i < count; i++) {
        zephyr::cstring::delete_copy(d[i]);
    }
    delete[] d;
}

}
