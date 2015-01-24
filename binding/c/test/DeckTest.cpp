#include <boost/test/unit_test.hpp>
#include <ygo/deck/c/Deck.h>
#include <ygo/deck/c/DB.h>

struct Deck_Fixture
{
    Deck_Fixture()
    {
        DB_NAME(set_path)("test/card.db");
        main = DECK_NAME(new)(ygo_data_DeckType_MAIN);
        side = DECK_NAME(new)(ygo_data_DeckType_SIDE);
        extra = DECK_NAME(new)(ygo_data_DeckType_EXTRA);
    }

    ~Deck_Fixture()
    {
        DECK_NAME(remove)(main);
        DECK_NAME(remove)(side);
        DECK_NAME(remove)(extra);
        DECK_NAME(delete)(main);
        DECK_NAME(delete)(side);
        DECK_NAME(delete)(extra);
    }

    DECK_THIS main;
    DECK_THIS side;
    DECK_THIS extra;
};

BOOST_FIXTURE_TEST_SUITE(Deck, Deck_Fixture)

BOOST_AUTO_TEST_CASE(AddCard)
{
    auto mainErr = DECK_NAME(addCard)(main, "Blue-Eyes White Dragon");
    BOOST_CHECK(mainErr == ygo_deck_DeckError_OK);
    int mainCount;
    auto mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 1);
    BOOST_CHECK_EQUAL(std::string(mainCards[0]->name),
            "Blue-Eyes White Dragon");
    DECK_NAME(delete_cards)(mainCards, mainCount);

    auto sideErr = DECK_NAME(addCard)(side, "Mirror Force");
    BOOST_CHECK(sideErr == ygo_deck_DeckError_OK);
    int sideCount;
    auto sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 1);
    BOOST_CHECK_EQUAL(std::string(sideCards[0]->name), "Mirror Force");
    DECK_NAME(delete_cards)(sideCards, sideCount);

    auto extraErr = DECK_NAME(addCard)(extra, "Stardust Dragon");
    BOOST_CHECK(extraErr == ygo_deck_DeckError_OK);
    int extraCount;
    auto extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 1);
    BOOST_CHECK_EQUAL(std::string(extraCards[0]->name), "Stardust Dragon");
    DECK_NAME(delete_cards)(extraCards, extraCount);
}

BOOST_AUTO_TEST_CASE(DeleteCard)
{
    DECK_NAME(addCard)(main, "Evilswarm Castor");
    int mainCount;
    auto mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 1);
    DECK_NAME(deleteCard)(main, "Evilswarm Castor");
    DECK_NAME(delete_cards)(mainCards, mainCount);
    mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 0);
    DECK_NAME(delete_cards)(mainCards, mainCount);

    DECK_NAME(addCard)(side, "Pot of Duality");
    int sideCount;
    auto sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 1);
    DECK_NAME(deleteCard)(side, "Pot of Duality");
    DECK_NAME(delete_cards)(sideCards, sideCount);
    sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 0);
    DECK_NAME(delete_cards)(sideCards, sideCount);

    DECK_NAME(addCard)(extra, "Abyss Dweller");
    int extraCount;
    auto extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 1);
    DECK_NAME(deleteCard)(extra, "Abyss Dweller");
    DECK_NAME(delete_cards)(extraCards, extraCount);
    extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 0);
    DECK_NAME(delete_cards)(extraCards, extraCount);
}

BOOST_AUTO_TEST_CASE(DeleteNotPresentCard)
{
    DECK_NAME(addCard)(main, "Evilswarm Castor");
    int mainCount;
    auto mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 1);
    DECK_NAME(deleteCard)(main, "Blue-Eyes White Dragon");
    DECK_NAME(delete_cards)(mainCards, mainCount);
    mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 1);
    DECK_NAME(delete_cards)(mainCards, mainCount);

    DECK_NAME(addCard)(side, "Pot of Duality");
    int sideCount;
    auto sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 1);
    DECK_NAME(deleteCard)(side, "Pot of Greed");
    DECK_NAME(delete_cards)(sideCards, sideCount);
    sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 1);
    DECK_NAME(delete_cards)(sideCards, sideCount);

    DECK_NAME(addCard)(extra, "Abyss Dweller");
    int extraCount;
    auto extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 1);
    DECK_NAME(deleteCard)(extra, "Gagaga Cowboy");
    DECK_NAME(delete_cards)(extraCards, extraCount);
    extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 1);
    DECK_NAME(delete_cards)(extraCards, extraCount);
}

BOOST_AUTO_TEST_CASE(DeckLimits)
{
    DECK_NAME(addCard)(main, "Evilswarm Castor");
    DECK_NAME(addCard)(main, "Evilswarm Castor");
    DECK_NAME(addCard)(main, "Evilswarm Castor");
    DECK_NAME(addCard)(main, "Evilswarm Heliotrope");
    DECK_NAME(addCard)(main, "Evilswarm Heliotrope");
    DECK_NAME(addCard)(main, "Evilswarm Heliotrope");
    DECK_NAME(addCard)(main, "Evilswarm Mandragora");
    DECK_NAME(addCard)(main, "Evilswarm Mandragora");
    DECK_NAME(addCard)(main, "Evilswarm Mandragora");
    DECK_NAME(addCard)(main, "Gravekeeper's Commandant");
    DECK_NAME(addCard)(main, "Gravekeeper's Commandant");
    DECK_NAME(addCard)(main, "Gravekeeper's Descendant");
    DECK_NAME(addCard)(main, "Gravekeeper's Spy");
    DECK_NAME(addCard)(main, "Gravekeeper's Spy");
    DECK_NAME(addCard)(main, "Gravekeeper's Spy");
    DECK_NAME(addCard)(main, "Rescue Rabbit");
    DECK_NAME(addCard)(main, "Thunder King Rai-Oh");
    DECK_NAME(addCard)(main, "Allure of Darkness");
    DECK_NAME(addCard)(main, "Dark Hole");
    DECK_NAME(addCard)(main, "Infestation Pandemic");
    DECK_NAME(addCard)(main, "Infestation Pandemic");
    DECK_NAME(addCard)(main, "Necrovalley");
    DECK_NAME(addCard)(main, "Necrovalley");
    DECK_NAME(addCard)(main, "Necrovalley");
    DECK_NAME(addCard)(main, "Pot of Duality");
    DECK_NAME(addCard)(main, "Pot of Duality");
    DECK_NAME(addCard)(main, "Reinforcement of the Army");
    DECK_NAME(addCard)(main, "Reinforcement of the Army");
    DECK_NAME(addCard)(main, "Bottomless Trap Hole");
    DECK_NAME(addCard)(main, "Compulsory Evacuation Device");
    DECK_NAME(addCard)(main, "Dimensional Prison");
    DECK_NAME(addCard)(main, "Dimensional Prison");
    DECK_NAME(addCard)(main, "Dimensional Prison");
    DECK_NAME(addCard)(main, "Fiendish Chain");
    DECK_NAME(addCard)(main, "Fiendish Chain");
    DECK_NAME(addCard)(main, "Infestation Infection");
    DECK_NAME(addCard)(main, "Solemn Warning");
    DECK_NAME(addCard)(main, "Torrential Tribute");
    DECK_NAME(addCard)(main, "Wiretap");
    DECK_NAME(addCard)(main, "Wiretap");
    int mainCount;
    auto mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 40);
    DECK_NAME(delete_cards)(mainCards, mainCount);

    DECK_NAME(addCard)(extra, "Abyss Dweller");
    DECK_NAME(addCard)(extra, "Cairngorgon, Antiluminescent Knight");
    DECK_NAME(addCard)(extra, "Evilswarm Bahamut");
    DECK_NAME(addCard)(extra, "Evilswarm Exciton Knight");
    DECK_NAME(addCard)(extra, "Evilswarm Ophion");
    DECK_NAME(addCard)(extra, "Evilswarm Ophion");
    DECK_NAME(addCard)(extra, "Evilswarm Ouroboros");
    DECK_NAME(addCard)(extra, "Evilswarm Thanatos");
    DECK_NAME(addCard)(extra, "Gagaga Cowboy");
    DECK_NAME(addCard)(extra, "Maestroke the Symphony Djinn");
    DECK_NAME(addCard)(extra, "Number 101: Silent Honor ARK");
    DECK_NAME(addCard)(extra, "Number 101: Silent Honor ARK");
    DECK_NAME(addCard)(extra, "Number 103: Ragnazero");
    DECK_NAME(addCard)(extra, "Number 66: Master Key Beetle");
    DECK_NAME(addCard)(extra, "Number 82: Heartlandraco");
    int extraCount;
    auto extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 15);
    DECK_NAME(delete_cards)(extraCards, extraCount);

    DECK_NAME(addCard)(side, "Trap Hole");
    DECK_NAME(addCard)(side, "White Hole");
    DECK_NAME(addCard)(side, "Debunk");
    DECK_NAME(addCard)(side, "Debunk");
    DECK_NAME(addCard)(side, "Mirror Force");
    DECK_NAME(addCard)(side, "Mirror Force");
    DECK_NAME(addCard)(side, "Mirror Force");
    DECK_NAME(addCard)(side, "Evilswarm Mandragora");
    DECK_NAME(addCard)(side, "Mind Control");
    DECK_NAME(addCard)(side, "Soul Release");
    DECK_NAME(addCard)(side, "Spiritualism");
    DECK_NAME(addCard)(side, "Spiritualism");
    DECK_NAME(addCard)(side, "Vanity's Emptiness");
    DECK_NAME(addCard)(side, "Vanity's Emptiness");
    DECK_NAME(addCard)(side, "Vanity's Emptiness");
    int sideCount;
    auto sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 15);
    DECK_NAME(delete_cards)(sideCards, sideCount);

    DECK_NAME(addCard)(main, "Archfiend Heiress");
    DECK_NAME(addCard)(main, "Armageddon Knight");
    DECK_NAME(addCard)(main, "Dark Grepher");
    DECK_NAME(addCard)(main, "Dark Grepher");
    DECK_NAME(addCard)(main, "Infernity Archfiend");
    DECK_NAME(addCard)(main, "Infernity Archfiend");
    DECK_NAME(addCard)(main, "Infernity Archfiend");
    DECK_NAME(addCard)(main, "Infernity Necromancer");
    DECK_NAME(addCard)(main, "Infernity Necromancer");
    DECK_NAME(addCard)(main, "Stygian Street Patrol");
    DECK_NAME(addCard)(main, "Stygian Street Patrol");
    DECK_NAME(addCard)(main, "Stygian Street Patrol");
    DECK_NAME(addCard)(main, "Summoner Monk");
    DECK_NAME(addCard)(main, "Summoner Monk");
    DECK_NAME(addCard)(main, "Infernity Barrier");
    DECK_NAME(addCard)(main, "Infernity Break");
    DECK_NAME(addCard)(main, "Infernity Break");
    DECK_NAME(addCard)(main, "Infernity Break");
    DECK_NAME(addCard)(main, "Trap Stun");
    DECK_NAME(addCard)(main, "Trap Stun");
    mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 60);
    DECK_NAME(delete_cards)(mainCards, mainCount);

    auto mainErr = DECK_NAME(addCard)(main, "Trap Stun");
    BOOST_CHECK(mainErr == ygo_deck_DeckError_DECK_FULL);
    mainCards = DECK_NAME(cards)(main, &mainCount);
    BOOST_CHECK_EQUAL(mainCount, 60);
    DECK_NAME(delete_cards)(mainCards, mainCount);

    auto extraErr = DECK_NAME(addCard)(extra, "Stardust Dragon");
    BOOST_CHECK(extraErr == ygo_deck_DeckError_DECK_FULL);
    extraCards = DECK_NAME(cards)(extra, &extraCount);
    BOOST_CHECK_EQUAL(extraCount, 15);
    DECK_NAME(delete_cards)(extraCards, extraCount);

    auto sideErr = DECK_NAME(addCard)(side, "Magic Cylinder");
    BOOST_CHECK(sideErr == ygo_deck_DeckError_DECK_FULL);
    sideCards = DECK_NAME(cards)(side, &sideCount);
    BOOST_CHECK_EQUAL(sideCount, 15);
    DECK_NAME(delete_cards)(sideCards, sideCount);
}

BOOST_AUTO_TEST_SUITE_END()
