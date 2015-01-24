#include <boost/test/unit_test.hpp>
#include <ygo/deck/c/User.h>
#include <ygo/deck/c/DB.h>

struct User_Fixture
{
    User_Fixture()
    {
        DB_NAME(set_path)("test/card.db");
    }
};

BOOST_FIXTURE_TEST_SUITE(User, User_Fixture)

BOOST_AUTO_TEST_CASE(Create)
{
    auto user = USER_NAME(new_create)("Test");
    auto name = USER_NAME(name)(user);
    BOOST_CHECK_EQUAL(std::string(name), "Test");
    USER_NAME(delete_name)(name);
    USER_NAME(delete)(user);
}

BOOST_AUTO_TEST_CASE(Open)
{
    auto user = USER_NAME(new)("Test");
    auto name = USER_NAME(name)(user);
    BOOST_CHECK_EQUAL(std::string(name), "Test");
    USER_NAME(delete_name)(name);
    USER_NAME(delete)(user);
}

BOOST_AUTO_TEST_CASE(Unknown)
{
    auto user = USER_NAME(new)("NotKnown");
    BOOST_CHECK(user == nullptr);
}

BOOST_AUTO_TEST_CASE(Remove)
{
    auto user = USER_NAME(new)("Test");
    auto name = USER_NAME(name)(user);
    BOOST_CHECK_EQUAL(std::string(name), "Test");
    USER_NAME(remove)(user);
    USER_NAME(delete_name)(name);
    USER_NAME(delete)(user);

    auto user2 = USER_NAME(new)("Test");
    BOOST_CHECK(user2 == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
