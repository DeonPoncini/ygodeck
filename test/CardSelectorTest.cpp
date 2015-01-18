#define BOOST_TEST_MODULE CardSelector
#include <boost/test/unit_test.hpp>
#include <ygo/deck/CardSelector.h>
#include <ygo/deck/DB.h>

#include <algorithm>

struct CardSelector_Fixture
{
    CardSelector_Fixture()
    {
        ygo::deck::DB::get().path("test/card.db");
    }

    ygo::deck::CardSelector cardSelector;
};

BOOST_FIXTURE_TEST_SUITE(CardSelector, CardSelector_Fixture)

BOOST_AUTO_TEST_CASE(Name)
{
    auto clist = cardSelector.name("Black").execute();
    for (auto&& c : clist) {
        BOOST_CHECK(c.find("Black") != std::string::npos);
    }
}

BOOST_AUTO_TEST_CASE(CardType)
{
    auto clist = cardSelector.name("Blue").cardType(
            ygo::data::CardType::SPELL).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.cardType == ygo::data::CardType::SPELL);
    }
}

BOOST_AUTO_TEST_CASE(Attribute)
{
    auto clist = cardSelector.name("Dragon").attribute(
            ygo::data::Attribute::DARK).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.attribute == ygo::data::Attribute::DARK);
    }
}

BOOST_AUTO_TEST_CASE(MonsterType)
{
    auto clist = cardSelector.name("Wind").monsterType(
            ygo::data::MonsterType::XYZ).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.monsterType == ygo::data::MonsterType::XYZ);
    }
}

BOOST_AUTO_TEST_CASE(Type)
{
    auto clist = cardSelector.name("saurus").type(
            ygo::data::Type::DINOSAUR).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.type == ygo::data::Type::DINOSAUR);
    }
}

BOOST_AUTO_TEST_CASE(Level)
{
    auto clist = cardSelector.level(11, mindbw::Operator::EQ).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.level == 11);
    }
}

BOOST_AUTO_TEST_CASE(Attack)
{
    auto clist = cardSelector.type(ygo::data::Type::DRAGON).
        attack(1000, mindbw::Operator::LT).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.attack < 1000);
    }
}

BOOST_AUTO_TEST_CASE(Defense)
{
    auto clist = cardSelector.type(ygo::data::Type::ROCK).
        defense(100, mindbw::Operator::LTE).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.defense <= 100);
    }
}

BOOST_AUTO_TEST_CASE(Pendulum)
{
    auto clist = cardSelector.lpendulum(8, mindbw::Operator::GTE)
        .rpendulum(8, mindbw::Operator::GTE).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.lpendulum >= 8);
        BOOST_CHECK(scd.rpendulum >= 8);
    }
}

BOOST_AUTO_TEST_CASE(SpellType)
{
    auto clist = cardSelector.name("Sky").spellType(
            ygo::data::SpellType::FIELD).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.spellType == ygo::data::SpellType::FIELD);
    }
}

BOOST_AUTO_TEST_CASE(TrapType)
{
    auto clist = cardSelector.name("Solemn").trapType(
            ygo::data::TrapType::COUNTER).execute();
    for (auto&& c : clist) {
        auto scd = cardSelector.query(c);
        BOOST_CHECK(scd.trapType == ygo::data::TrapType::COUNTER);
    }
}

BOOST_AUTO_TEST_SUITE_END()
