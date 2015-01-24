#include <boost/test/unit_test.hpp>
#include <ygo/deck/c/DeckSet.h>
#include <ygo/deck/c/DB.h>

struct DeckSet_Fixture
{
    DeckSet_Fixture()
    {
        DB_NAME(set_path)("test/card.db");
        auto user = USER_NAME(new_create)("DeckSetUser");
        auto format = FORMAT_NAME(new)(ygo_data_Format_ADVANCED, "April 2004");
        deckSet = DECKSET_NAME(create)("DeckSetTest", user, format);
        USER_NAME(delete)(user);
        FORMAT_NAME(delete)(format);
    }

    ~DeckSet_Fixture()
    {
        DECKSET_NAME(remove)(deckSet);
        DECKSET_NAME(delete)(deckSet);
    }

    DECKSET_THIS deckSet;
};

BOOST_FIXTURE_TEST_SUITE(DeckSet, DeckSet_Fixture)

BOOST_AUTO_TEST_CASE(AddCard)
{
    auto mainErr = DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN,
            "Blue-Eyes White Dragon");
    BOOST_CHECK(mainErr == ygo_deck_DeckError_OK);

    auto sideErr = DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE,
            "Mirror Force");
    BOOST_CHECK(sideErr == ygo_deck_DeckError_OK);

    auto extraErr = DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA,
            "Stardust Dragon");
    BOOST_CHECK(extraErr == ygo_deck_DeckError_OK);

    auto cards = DECKSET_NAME(cards)(deckSet);
    BOOST_CHECK_EQUAL(cards.main_count, 1);
    BOOST_CHECK_EQUAL(std::string(cards.main[0]->name), "Blue-Eyes White Dragon");
    BOOST_CHECK_EQUAL(cards.side_count, 1);
    BOOST_CHECK_EQUAL(std::string(cards.side[0]->name), "Mirror Force");
    BOOST_CHECK_EQUAL(cards.extra_count, 1);
    BOOST_CHECK_EQUAL(std::string(cards.extra[0]->name), "Stardust Dragon");
    DECKSET_NAME(delete_cards)(cards);
}

BOOST_AUTO_TEST_CASE(DeleteCard)
{
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Castor");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Pot of Duality");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Abyss Dweller");

    auto cards = DECKSET_NAME(cards)(deckSet);
    BOOST_CHECK_EQUAL(cards.main_count, 1);
    BOOST_CHECK_EQUAL(cards.side_count, 1);
    BOOST_CHECK_EQUAL(cards.extra_count, 1);
    DECKSET_NAME(delete_cards)(cards);

    DECKSET_NAME(deleteCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Castor");
    DECKSET_NAME(deleteCard)(deckSet, ygo_data_DeckType_SIDE, "Pot of Duality");
    DECKSET_NAME(deleteCard)(deckSet, ygo_data_DeckType_EXTRA, "Abyss Dweller");

    cards = DECKSET_NAME(cards)(deckSet);
    BOOST_CHECK_EQUAL(cards.main_count, 0);
    BOOST_CHECK_EQUAL(cards.side_count, 0);
    BOOST_CHECK_EQUAL(cards.extra_count, 0);
    DECKSET_NAME(delete_cards)(cards);
}

BOOST_AUTO_TEST_CASE(DeleteNotPresentCard)
{
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Castor");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Pot of Duality");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Abyss Dweller");

    auto cards = DECKSET_NAME(cards)(deckSet);
    BOOST_CHECK_EQUAL(cards.main_count, 1);
    BOOST_CHECK_EQUAL(cards.side_count, 1);
    BOOST_CHECK_EQUAL(cards.extra_count, 1);
    DECKSET_NAME(delete_cards)(cards);

    DECKSET_NAME(deleteCard)(deckSet, ygo_data_DeckType_MAIN,
            "Blue-Eyes White Dragon");
    DECKSET_NAME(deleteCard)(deckSet, ygo_data_DeckType_SIDE, "Pot of Greed");
    DECKSET_NAME(deleteCard)(deckSet, ygo_data_DeckType_EXTRA, "Gagaga Cowboy");

    cards = DECKSET_NAME(cards)(deckSet);
    BOOST_CHECK_EQUAL(cards.main_count, 1);
    BOOST_CHECK_EQUAL(cards.side_count, 1);
    BOOST_CHECK_EQUAL(cards.extra_count, 1);
    DECKSET_NAME(delete_cards)(cards);
}

BOOST_AUTO_TEST_CASE(Validate)
{
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Castor");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Castor");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Castor");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Heliotrope");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Heliotrope");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Heliotrope");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Mandragora");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Mandragora");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Evilswarm Mandragora");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Gravekeeper's Commandant");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Gravekeeper's Commandant");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Gravekeeper's Descendant");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Gravekeeper's Spy");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Gravekeeper's Spy");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Gravekeeper's Spy");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Rescue Rabbit");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Thunder King Rai-Oh");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Allure of Darkness");

    BOOST_CHECK_EQUAL(DECKSET_NAME(validate)(deckSet), 0);

    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Dark Hole");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infestation Pandemic");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infestation Pandemic");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Necrovalley");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Necrovalley");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Necrovalley");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Pot of Duality");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Pot of Duality");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Reinforcement of the Army");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Reinforcement of the Army");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Bottomless Trap Hole");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Compulsory Evacuation Device");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Dimensional Prison");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Dimensional Prison");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Dimensional Prison");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Fiendish Chain");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Fiendish Chain");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infestation Infection");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Solemn Warning");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Torrential Tribute");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Wiretap");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Wiretap");

    BOOST_CHECK_EQUAL(DECKSET_NAME(validate)(deckSet), 1);

    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Abyss Dweller");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Cairngorgon, Antiluminescent Knight");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Evilswarm Bahamut");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Evilswarm Exciton Knight");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Evilswarm Ophion");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Evilswarm Ophion");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Evilswarm Ouroboros");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Evilswarm Thanatos");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Gagaga Cowboy");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Maestroke the Symphony Djinn");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Number 101: Silent Honor ARK");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Number 101: Silent Honor ARK");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Number 103: Ragnazero");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Number 66: Master Key Beetle");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_EXTRA, "Number 82: Heartlandraco");

    BOOST_CHECK_EQUAL(DECKSET_NAME(validate)(deckSet), 1);

    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Trap Hole");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "White Hole");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Debunk");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Debunk");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Mirror Force");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Mirror Force");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Mirror Force");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Evilswarm Mandragora");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Mind Control");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Soul Release");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Spiritualism");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Spiritualism");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Vanity's Emptiness");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Vanity's Emptiness");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_SIDE, "Vanity's Emptiness");

    BOOST_CHECK_EQUAL(DECKSET_NAME(validate)(deckSet), 1);

    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Archfiend Heiress");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Armageddon Knight");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Dark Grepher");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Dark Grepher");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Archfiend");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Archfiend");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Archfiend");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Necromancer");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Necromancer");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Stygian Street Patrol");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Stygian Street Patrol");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Stygian Street Patrol");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Summoner Monk");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Summoner Monk");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Barrier");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Break");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Break");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Infernity Break");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Trap Stun");
    DECKSET_NAME(addCard)(deckSet, ygo_data_DeckType_MAIN, "Trap Stun");

    BOOST_CHECK_EQUAL(DECKSET_NAME(validate)(deckSet), 1);
}

BOOST_AUTO_TEST_SUITE_END()
