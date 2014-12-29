#include <ygo/deck/CardSelector.h>
#include <c/ygo/deck/CardSelector.h>

#include <ygo/data/CardData.h>

#include <zephyr/cstring.h>

extern "C" {

#define C_CAST(p) reinterpret_cast<CARD_SEL_THIS>(p)
#define CPP_CAST(p) reinterpret_cast<ygo::deck::CardSelector*>(p)

CARD_SEL_THIS CARD_SEL(new)(void)
{
    return C_CAST(new ygo::deck::CardSelector());
}

void CARD_SEL(delete)(CARD_SEL_THIS p)
{
    delete CPP_CAST(p);
}

ygo_data_C_StaticCardData*
CARD_SEL(query)(CARD_SEL_THIS p, const char* name)
{
    return ygo::data::staticDataToC(CPP_CAST(p)->query(name));
}

void CARD_SEL(query_delete)(ygo_data_C_StaticCardData* p)
{
    ygo::data::deleteCStaticData(p);
}

char** CARD_SEL(execute)(CARD_SEL_THIS p, int* items)
{
    // convert a vector of strings into an array of char* pointers
    auto list = CPP_CAST(p)->execute();
    *items = list.size();
    if (*items > 0) {
        char** ret = new char*[*items];
        for (auto i = 0; i < *items; i++) {
            ret[i] = zephyr::cstring::make_copy(list[i]);
        }
        return ret;
    } else {
        return nullptr;
    }
}

void CARD_SEL(execute_delete) (char** e, int items)
{
    for (auto i = 0; i < items; i++) {
        zephyr::cstring::delete_copy(e[i]);
    }
    delete [] e;
}

CARD_SEL_THIS CARD_SEL(name)(CARD_SEL_THIS p, const char* like)
{
    CPP_CAST(p)->name(like);
    return p;
}

CARD_SEL_THIS CARD_SEL(cardType)(CARD_SEL_THIS p, ZU(ygo)ZU(data)CardType ct)
{
    CPP_CAST(p)->cardType(ct);
    return p;
}

CARD_SEL_THIS CARD_SEL(attribute)(CARD_SEL_THIS p, ZU(ygo)ZU(data)Attribute a)
{
    CPP_CAST(p)->attribute(a);
    return p;
}

CARD_SEL_THIS CARD_SEL(monsterType)(CARD_SEL_THIS p, ZU(ygo)ZU(data)MonsterType mt)
{
    CPP_CAST(p)->monsterType(mt);
    return p;
}

CARD_SEL_THIS CARD_SEL(type)(CARD_SEL_THIS p, ZU(ygo)ZU(data)Type t)
{
    CPP_CAST(p)->type(t);
    return p;
}

CARD_SEL_THIS CARD_SEL(level)(CARD_SEL_THIS p, int l, ZU(mindbw)Operator op)
{
    CPP_CAST(p)->level(l, op);
    return p;
}

CARD_SEL_THIS CARD_SEL(attack)(CARD_SEL_THIS p, int a, ZU(mindbw)Operator op)
{
    CPP_CAST(p)->attack(a, op);
    return p;
}

CARD_SEL_THIS CARD_SEL(defense)(CARD_SEL_THIS p, int d, ZU(mindbw)Operator op)
{
    CPP_CAST(p)->defense(d, op);
    return p;
}

CARD_SEL_THIS CARD_SEL(lpendulum)(CARD_SEL_THIS p, int l, ZU(mindbw)Operator op)
{
    CPP_CAST(p)->lpendulum(l, op);
    return p;
}

CARD_SEL_THIS CARD_SEL(rpendulum)(CARD_SEL_THIS p, int r, ZU(mindbw)Operator op)
{
    CPP_CAST(p)->rpendulum(r, op);
    return p;
}

CARD_SEL_THIS CARD_SEL(spellType)(CARD_SEL_THIS p, ZU(ygo)ZU(data)SpellType st)
{
    CPP_CAST(p)->spellType(st);
    return p;
}

CARD_SEL_THIS CARD_SEL(trapType)(CARD_SEL_THIS p, ZU(ygo)ZU(data)TrapType tt)
{
    CPP_CAST(p)->trapType(tt);
    return p;
}

}
