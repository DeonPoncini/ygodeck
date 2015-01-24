#include <boost/test/unit_test.hpp>
#include <ygo/deck/c/CardSelector.h>
#include <ygo/deck/c/DB.h>

#include <algorithm>

struct CardSelector_Fixture
{
    CardSelector_Fixture()
    {
        DB_NAME(set_path)("test/card.db");
        cardSelector = CARD_SEL(new)();
    }

    ~CardSelector_Fixture()
    {
        CARD_SEL(delete)(cardSelector);
    }

    CARD_SEL_THIS cardSelector;
};

BOOST_FIXTURE_TEST_SUITE(CardSelector, CardSelector_Fixture)

BOOST_AUTO_TEST_CASE(Name)
{
    int items;
    CARD_SEL(name)(cardSelector, "Black");
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        BOOST_CHECK(std::string(clist[i]).find("Black") != std::string::npos);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(CardType)
{
    int items;
    CARD_SEL(name)(cardSelector, "Blue");
    CARD_SEL(cardType)(cardSelector, ygo_data_CardType_SPELL);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->cardType == ygo_data_CardType_SPELL);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(Attribute)
{
    int items;
    CARD_SEL(name)(cardSelector, "Dragon");
    CARD_SEL(attribute)(cardSelector, ygo_data_Attribute_DARK);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->attribute == ygo_data_Attribute_DARK);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(MonsterType)
{
    int items;
    CARD_SEL(name)(cardSelector, "Wind");
    CARD_SEL(monsterType)(cardSelector, ygo_data_MonsterType_XYZ);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->monsterType == ygo_data_MonsterType_XYZ);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(Type)
{
    int items;
    CARD_SEL(name)(cardSelector, "saurus");
    CARD_SEL(type)(cardSelector, ygo_data_Type_DINOSAUR);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->type == ygo_data_Type_DINOSAUR);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(Level)
{
    int items;
    CARD_SEL(level)(cardSelector, 11, mindbw_Operator_EQ);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK_EQUAL(scd->level, 11);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(Attack)
{
    int items;
    CARD_SEL(type)(cardSelector, ygo_data_Type_DRAGON);
    CARD_SEL(attack)(cardSelector, 1000, mindbw_Operator_LT);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->attack < 1000);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(Defense)
{
    int items;
    CARD_SEL(type)(cardSelector, ygo_data_Type_ROCK);
    CARD_SEL(defense)(cardSelector, 100, mindbw_Operator_LTE);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->defense <= 100);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(Pendulum)
{
    int items;
    CARD_SEL(lpendulum)(cardSelector, 8, mindbw_Operator_GTE);
    CARD_SEL(rpendulum)(cardSelector, 8, mindbw_Operator_GTE);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->lpendulum <= 100);
        BOOST_CHECK(scd->rpendulum <= 100);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(SpellType)
{
    int items;
    CARD_SEL(name)(cardSelector, "Sky");
    CARD_SEL(spellType)(cardSelector, ygo_data_SpellType_FIELD);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->spellType == ygo_data_SpellType_FIELD);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_CASE(TrapType)
{
    int items;
    CARD_SEL(name)(cardSelector, "Solemn");
    CARD_SEL(trapType)(cardSelector, ygo_data_TrapType_COUNTER);
    auto clist = CARD_SEL(execute)(cardSelector, &items);
    for (auto i = 0; i < items; i++) {
        auto scd = CARD_SEL(query)(cardSelector, clist[i]);
        BOOST_CHECK(scd->trapType == ygo_data_TrapType_COUNTER);
        CARD_SEL(query_delete)(scd);
    }
    CARD_SEL(execute_delete)(clist, items);
}

BOOST_AUTO_TEST_SUITE_END()
