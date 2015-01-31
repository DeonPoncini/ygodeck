#include <boost/test/unit_test.hpp>
#include <ygo/deck/Format.h>
#include <ygo/deck/DB.h>

struct Format_Fixture
{
    Format_Fixture()
    {
        ygo::deck::DB::get().path("test/card.db");
    }
};

BOOST_FIXTURE_TEST_SUITE(Format, Format_Fixture)

BOOST_AUTO_TEST_CASE(Create)
{
    auto formatDates = ygo::deck::Format::formatDates();
    for (auto&& f : formatDates) {
        ygo::deck::Format formatA(ygo::data::Format::ADVANCED, f);
        BOOST_CHECK_EQUAL(formatA.formatDate(), f);
        BOOST_CHECK(formatA.format() == ygo::data::Format::ADVANCED);

        ygo::deck::Format formatT(ygo::data::Format::TRADITIONAL, f);
        BOOST_CHECK_EQUAL(formatT.formatDate(), f);
        BOOST_CHECK(formatT.format() == ygo::data::Format::TRADITIONAL);

        ygo::deck::Format formatM(ygo::data::Format::MEGABANNED, f);
        BOOST_CHECK_EQUAL(formatM.formatDate(), f);
        BOOST_CHECK(formatM.format() == ygo::data::Format::MEGABANNED);
    }
}

BOOST_AUTO_TEST_CASE(Invalid)
{
    try {
        ygo::deck::Format f(ygo::data::Format::ADVANCED, "InvalidFormat");
        BOOST_CHECK_EQUAL(0, 1);
    } catch (std::runtime_error& e) {
        BOOST_CHECK_EQUAL(1, 1);
    }
}

BOOST_AUTO_TEST_CASE(Limits)
{
    ygo::deck::Format formatA(ygo::data::Format::ADVANCED, "April 2004");
    ygo::deck::Format formatT(ygo::data::Format::TRADITIONAL, "April 2004");
    ygo::deck::Format formatM(ygo::data::Format::MEGABANNED, "April 2004");

    BOOST_CHECK_EQUAL(0, formatA.cardCount("Change of Heart"));
    BOOST_CHECK_EQUAL(1, formatT.cardCount("Change of Heart"));
    BOOST_CHECK_EQUAL(0, formatM.cardCount("Change of Heart"));

    BOOST_CHECK_EQUAL(1, formatA.cardCount("Mage Power"));
    BOOST_CHECK_EQUAL(1, formatT.cardCount("Mage Power"));
    BOOST_CHECK_EQUAL(0, formatM.cardCount("Mage Power"));

    BOOST_CHECK_EQUAL(2, formatA.cardCount("Creature Swap"));
    BOOST_CHECK_EQUAL(2, formatT.cardCount("Creature Swap"));
    BOOST_CHECK_EQUAL(0, formatM.cardCount("Creature Swap"));

    BOOST_CHECK_EQUAL(3, formatA.cardCount("Kuriboh"));
    BOOST_CHECK_EQUAL(3, formatT.cardCount("Kuriboh"));
    BOOST_CHECK_EQUAL(3, formatM.cardCount("Kuriboh"));
}

BOOST_AUTO_TEST_SUITE_END()
