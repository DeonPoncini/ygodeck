#include <ygo/deck/User.h>
#include <c/ygo/deck/User.h>

#include <ygo/deck/DeckSet.h>
#include <c/ygo/deck/DeckSet.h>

#include <zephyr/cstring.h>

extern "C" {

#define C_CAST(p) reinterpret_cast<USER_THIS>(p)
#define CPP_CAST(p) reinterpret_cast<ygo::deck::User*>(p)

USER_THIS USER_NAME(new)(const char* name)
{
    return C_CAST(new ygo::deck::User(name));
}

USER_THIS USER_NAME(new_create)(const char* name)
{
    return C_CAST(new ygo::deck::User(name, true));
}

void USER_NAME(delete)(USER_THIS p)
{
    delete CPP_CAST(p);
}

char* USER_NAME(name)(USER_THIS p)
{
    return zephyr::cstring::make_copy(CPP_CAST(p)->name());
}

void USER_NAME(delete_name)(char* name)
{
    zephyr::cstring::delete_copy(name);
}

char* USER_NAME(id)(USER_THIS p)
{
    return zephyr::cstring::make_copy(CPP_CAST(p)->id());
}

void USER_NAME(delete_id)(char* id)
{
    zephyr::cstring::delete_copy(id);
}

ygo_deck_DeckSet** USER_NAME(deckSets)(USER_THIS p, int* count)
{
    auto sets = CPP_CAST(p)->deckSets();
    *count = sets.size();
    auto ret = new ygo_deck_DeckSet*[*count];
    for (auto i = 0; i < *count; i++) {
        ret[i] = reinterpret_cast<ygo_deck_DeckSet*>(
                new ygo::deck::DeckSet(sets[i]));
    }
    return ret;
}

void USER_NAME(delete_deckSets)(ygo_deck_DeckSet** d, int count)
{
    for (auto i = 0; i < count; i++) {
        delete reinterpret_cast<ygo::deck::DeckSet*>(d[i]);
    }
    delete[] d;
}

}
