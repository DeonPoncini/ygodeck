#include <boost/test/unit_test.hpp>
#include <ygo/deck/Deck.h>
#include <ygo/deck/DB.h>

struct Deck_Fixture
{
    Deck_Fixture()
    {
        ygo::deck::DB::get().path("test/card.db");
        main = new ygo::deck::Deck(ygo::data::DeckType::MAIN);
        side = new ygo::deck::Deck(ygo::data::DeckType::SIDE);
        extra = new ygo::deck::Deck(ygo::data::DeckType::EXTRA);
    }

    ~Deck_Fixture()
    {
        delete main;
        delete side;
        delete extra;
    }

    ygo::deck::Deck* main;
    ygo::deck::Deck* side;
    ygo::deck::Deck* extra;
};

BOOST_FIXTURE_TEST_SUITE(Deck, Deck_Fixture)

BOOST_AUTO_TEST_CASE(AddCard)
{
    auto mainErr = main->addCard("Blue-Eyes White Dragon");
    BOOST_CHECK(mainErr == ygo::deck::DeckError::OK);
    auto mainCards = main->cards();
    BOOST_CHECK(mainCards.size() == 1);
    BOOST_CHECK(mainCards[0].name == "Blue-Eyes White Dragon");

    auto sideErr = side->addCard("Mirror Force");
    BOOST_CHECK(sideErr == ygo::deck::DeckError::OK);
    auto sideCards = side->cards();
    BOOST_CHECK(sideCards.size() == 1);
    BOOST_CHECK(sideCards[0].name == "Mirror Force");

    auto extraErr = extra->addCard("Stardust Dragon");
    BOOST_CHECK(extraErr == ygo::deck::DeckError::OK);
    auto extraCards = extra->cards();
    BOOST_CHECK(extraCards.size() == 1);
    BOOST_CHECK(extraCards[0].name == "Stardust Dragon");
}

BOOST_AUTO_TEST_CASE(DeleteCard)
{
    main->addCard("Evilswarm Castor");
    auto mainCards = main->cards();
    BOOST_CHECK(mainCards.size() == 1);
    main->deleteCard("Evilswarm Castor");
    mainCards = main->cards();
    BOOST_CHECK(mainCards.size() == 0);

    side->addCard("Pot of Duality");
    auto sideCards = side->cards();
    BOOST_CHECK(sideCards.size() == 1);
    side->deleteCard("Pot of Duality");
    sideCards = side->cards();
    BOOST_CHECK(sideCards.size() == 0);

    extra->addCard("Abyss Dweller");
    auto extraCards = extra->cards();
    BOOST_CHECK(extraCards.size() == 1);
    extra->deleteCard("Abyss Dweller");
    extraCards = extra->cards();
    BOOST_CHECK(extraCards.size() == 0);
}

BOOST_AUTO_TEST_CASE(DeleteNotPresentCard)
{
    main->addCard("Evilswarm Castor");
    auto mainCards = main->cards();
    BOOST_CHECK(mainCards.size() == 1);
    main->deleteCard("Blue-Eyes White Dragon");
    mainCards = main->cards();
    BOOST_CHECK(mainCards.size() == 1);

    side->addCard("Pot of Duality");
    auto sideCards = side->cards();
    BOOST_CHECK(sideCards.size() == 1);
    side->deleteCard("Pot of Greed");
    sideCards = side->cards();
    BOOST_CHECK(sideCards.size() == 1);

    extra->addCard("Abyss Dweller");
    auto extraCards = extra->cards();
    BOOST_CHECK(extraCards.size() == 1);
    extra->deleteCard("Gagaga Cowboy");
    extraCards = extra->cards();
    BOOST_CHECK(extraCards.size() == 1);
}

BOOST_AUTO_TEST_CASE(DeckLimits)
{
    main->addCard("Evilswarm Castor");
    main->addCard("Evilswarm Castor");
    main->addCard("Evilswarm Castor");
    main->addCard("Evilswarm Heliotrope");
    main->addCard("Evilswarm Heliotrope");
    main->addCard("Evilswarm Heliotrope");
    main->addCard("Evilswarm Mandragora");
    main->addCard("Evilswarm Mandragora");
    main->addCard("Evilswarm Mandragora");
    main->addCard("Gravekeeper's Commandant");
    main->addCard("Gravekeeper's Commandant");
    main->addCard("Gravekeeper's Descendant");
    main->addCard("Gravekeeper's Spy");
    main->addCard("Gravekeeper's Spy");
    main->addCard("Gravekeeper's Spy");
    main->addCard("Rescue Rabbit");
    main->addCard("Thunder King Rai-Oh");
    main->addCard("Allure of Darkness");
    main->addCard("Dark Hole");
    main->addCard("Infestation Pandemic");
    main->addCard("Infestation Pandemic");
    main->addCard("Necrovalley");
    main->addCard("Necrovalley");
    main->addCard("Necrovalley");
    main->addCard("Pot of Duality");
    main->addCard("Pot of Duality");
    main->addCard("Reinforcement of the Army");
    main->addCard("Reinforcement of the Army");
    main->addCard("Bottomless Trap Hole");
    main->addCard("Compulsory Evacuation Device");
    main->addCard("Dimensional Prison");
    main->addCard("Dimensional Prison");
    main->addCard("Dimensional Prison");
    main->addCard("Fiendish Chain");
    main->addCard("Fiendish Chain");
    main->addCard("Infestation Infection");
    main->addCard("Solemn Warning");
    main->addCard("Torrential Tribute");
    main->addCard("Wiretap");
    main->addCard("Wiretap");
    auto mainCards = main->cards();
    BOOST_CHECK_EQUAL(mainCards.size(), 40);

    extra->addCard("Abyss Dweller");
    extra->addCard("Cairngorgon, Antiluminescent Knight");
    extra->addCard("Evilswarm Bahamut");
    extra->addCard("Evilswarm Exciton Knight");
    extra->addCard("Evilswarm Ophion");
    extra->addCard("Evilswarm Ophion");
    extra->addCard("Evilswarm Ouroboros");
    extra->addCard("Evilswarm Thanatos");
    extra->addCard("Gagaga Cowboy");
    extra->addCard("Maestroke the Symphony Djinn");
    extra->addCard("Number 101: Silent Honor ARK");
    extra->addCard("Number 101: Silent Honor ARK");
    extra->addCard("Number 103: Ragnazero");
    extra->addCard("Number 66: Master Key Beetle");
    extra->addCard("Number 82: Heartlandraco");
    auto extraCards = extra->cards();
    BOOST_CHECK_EQUAL(extraCards.size(), 15);

    side->addCard("Trap Hole");
    side->addCard("White Hole");
    side->addCard("Debunk");
    side->addCard("Debunk");
    side->addCard("Mirror Force");
    side->addCard("Mirror Force");
    side->addCard("Mirror Force");
    side->addCard("Evilswarm Mandragora");
    side->addCard("Mind Control");
    side->addCard("Soul Release");
    side->addCard("Spiritualism");
    side->addCard("Spiritualism");
    side->addCard("Vanity's Emptiness");
    side->addCard("Vanity's Emptiness");
    side->addCard("Vanity's Emptiness");
    auto sideCards = side->cards();
    BOOST_CHECK_EQUAL(sideCards.size(), 15);

    main->addCard("Archfiend Heiress");
    main->addCard("Armageddon Knight");
    main->addCard("Dark Grepher");
    main->addCard("Dark Grepher");
    main->addCard("Infernity Archfiend");
    main->addCard("Infernity Archfiend");
    main->addCard("Infernity Archfiend");
    main->addCard("Infernity Necromancer");
    main->addCard("Infernity Necromancer");
    main->addCard("Stygian Street Patrol");
    main->addCard("Stygian Street Patrol");
    main->addCard("Stygian Street Patrol");
    main->addCard("Summoner Monk");
    main->addCard("Summoner Monk");
    main->addCard("Infernity Barrier");
    main->addCard("Infernity Break");
    main->addCard("Infernity Break");
    main->addCard("Infernity Break");
    main->addCard("Trap Stun");
    main->addCard("Trap Stun");
    mainCards = main->cards();
    BOOST_CHECK_EQUAL(mainCards.size(), 60);

    auto mainErr = main->addCard("Trap Stun");
    BOOST_CHECK(mainErr == ygo::deck::DeckError::DECK_FULL);
    mainCards = main->cards();
    BOOST_CHECK_EQUAL(mainCards.size(), 60);

    auto extraErr = extra->addCard("Stardust Dragon");
    BOOST_CHECK(extraErr == ygo::deck::DeckError::DECK_FULL);
    extraCards = extra->cards();
    BOOST_CHECK_EQUAL(extraCards.size(), 15);

    auto sideErr = side->addCard("Magic Cylinder");
    BOOST_CHECK(sideErr == ygo::deck::DeckError::DECK_FULL);
    sideCards = side->cards();
    BOOST_CHECK_EQUAL(sideCards.size(), 15);
}

BOOST_AUTO_TEST_SUITE_END()
