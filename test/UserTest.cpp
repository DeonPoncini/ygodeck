#include <boost/test/unit_test.hpp>
#include <ygo/deck/User.h>
#include <ygo/deck/DB.h>

struct User_Fixture
{
    User_Fixture()
    {
        ygo::deck::DB::get().path("test/card.db");
    }
};

BOOST_FIXTURE_TEST_SUITE(User, User_Fixture)

BOOST_AUTO_TEST_CASE(Create)
{
    ygo::deck::User user("Test", true);
    BOOST_CHECK_EQUAL(user.name(), "Test");
}

BOOST_AUTO_TEST_CASE(Open)
{
    ygo::deck::User user("Test");
    BOOST_CHECK_EQUAL(user.name(), "Test");
}

BOOST_AUTO_TEST_CASE(Unknown)
{
    try {
        ygo::deck::User user("NotKnown");
        BOOST_CHECK_EQUAL(0, 1);
    } catch (std::runtime_error& e) {
        BOOST_CHECK_EQUAL(1, 1);
    }
}

BOOST_AUTO_TEST_CASE(Remove)
{
    ygo::deck::User user("Test");
    BOOST_CHECK_EQUAL(user.name(), "Test");
    user.remove();
    try {
        ygo::deck::User user2("Test");
        BOOST_CHECK_EQUAL(0, 1);
    } catch (std::runtime_error& e) {
        BOOST_CHECK_EQUAL(1, 1);
    }
}

BOOST_AUTO_TEST_SUITE_END()
