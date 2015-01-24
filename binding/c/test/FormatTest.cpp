#include <boost/test/unit_test.hpp>
#include <ygo/deck/c/Format.h>
#include <ygo/deck/c/DB.h>

struct Format_Fixture
{
    Format_Fixture()
    {
        DB_NAME(set_path)("test/card.db");
    }
};

BOOST_FIXTURE_TEST_SUITE(Format, Format_Fixture)

BOOST_AUTO_TEST_CASE(Create)
{
    int count;
    auto formatDates = FORMAT_NAME(formatDates)(&count);
    for (auto i = 0; i < count; i++) {
        auto formatA = FORMAT_NAME(new)(ygo_data_Format_ADVANCED,
                formatDates[i]);
        auto formatDateA = FORMAT_NAME(formatDate)(formatA);
        auto formatFA = FORMAT_NAME(format)(formatA);
        BOOST_CHECK_EQUAL(std::string(formatDateA), formatDates[i]);
        BOOST_CHECK(formatFA == ygo_data_Format_ADVANCED);
        FORMAT_NAME(delete_formatDate)(formatDateA);

        auto formatT = FORMAT_NAME(new)(ygo_data_Format_TRADITIONAL,
                formatDates[i]);
        auto formatDateT = FORMAT_NAME(formatDate)(formatT);
        auto formatTA = FORMAT_NAME(format)(formatT);
        BOOST_CHECK_EQUAL(std::string(formatDateT), formatDates[i]);
        BOOST_CHECK(formatTA == ygo_data_Format_TRADITIONAL);
        FORMAT_NAME(delete_formatDate)(formatDateT);
    }
    FORMAT_NAME(delete_formatDates)(formatDates, count);
}

BOOST_AUTO_TEST_CASE(Invalid)
{
    auto format = FORMAT_NAME(new)(ygo_data_Format_ADVANCED, "InvalidFormat");
    BOOST_CHECK(format == nullptr);
}

BOOST_AUTO_TEST_CASE(Limits)
{
    auto formatA = FORMAT_NAME(new)(ygo_data_Format_ADVANCED, "April 2004");
    auto formatT = FORMAT_NAME(new)(ygo_data_Format_TRADITIONAL, "April 2004");

    BOOST_CHECK_EQUAL(0, FORMAT_NAME(cardCount)(formatA, "Change of Heart"));
    BOOST_CHECK_EQUAL(1, FORMAT_NAME(cardCount)(formatT, "Change of Heart"));

    BOOST_CHECK_EQUAL(1, FORMAT_NAME(cardCount)(formatA, "Mage Power"));
    BOOST_CHECK_EQUAL(1, FORMAT_NAME(cardCount)(formatT, "Mage Power"));

    BOOST_CHECK_EQUAL(2, FORMAT_NAME(cardCount)(formatA, "Creature Swap"));
    BOOST_CHECK_EQUAL(2, FORMAT_NAME(cardCount)(formatT, "Creature Swap"));

    BOOST_CHECK_EQUAL(3, FORMAT_NAME(cardCount)(formatA, "Kuriboh"));
    BOOST_CHECK_EQUAL(3, FORMAT_NAME(cardCount)(formatT, "Kuriboh"));

    FORMAT_NAME(delete)(formatA);
    FORMAT_NAME(delete)(formatT);

}

BOOST_AUTO_TEST_SUITE_END()
