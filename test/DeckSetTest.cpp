#include <boost/test/unit_test.hpp>
#include <ygo/deck/DeckSet.h>
#include <ygo/deck/DB.h>

struct DeckSet_Fixture
{
    DeckSet_Fixture()
    {
        ygo::deck::DB::get().path("test/card.db");
        ygo::deck::User user("DeckSetUser", true);
        ygo::deck::Format format(ygo::data::Format::ADVANCED, "April 2004");
        deckSet = new ygo::deck::DeckSet("DeckSetTest", user, format, true);
    }

    ~DeckSet_Fixture()
    {
        deckSet->remove();
        delete deckSet;
    }

    ygo::deck::DeckSet* deckSet;
};

BOOST_FIXTURE_TEST_SUITE(DeckSet, DeckSet_Fixture)

BOOST_AUTO_TEST_CASE(AddCard)
{
    auto mainErr = deckSet->addCard(ygo::data::DeckType::MAIN,
            "Blue-Eyes White Dragon");
    BOOST_CHECK(mainErr == ygo::deck::DeckError::OK);

    auto sideErr = deckSet->addCard(ygo::data::DeckType::SIDE,
            "Mirror Force");
    BOOST_CHECK(sideErr == ygo::deck::DeckError::OK);

    auto extraErr = deckSet->addCard(ygo::data::DeckType::EXTRA,
            "Stardust Dragon");
    BOOST_CHECK(extraErr == ygo::deck::DeckError::OK);

    auto cards = deckSet->cards();
    auto main = cards[ygo::data::DeckType::MAIN];
    auto side = cards[ygo::data::DeckType::SIDE];
    auto extra = cards[ygo::data::DeckType::EXTRA];

    BOOST_CHECK_EQUAL(main.size(), 1);
    BOOST_CHECK_EQUAL(main[0].name, "Blue-Eyes White Dragon");
    BOOST_CHECK_EQUAL(side.size(), 1);
    BOOST_CHECK_EQUAL(side[0].name, "Mirror Force");
    BOOST_CHECK_EQUAL(extra.size(), 1);
    BOOST_CHECK_EQUAL(extra[0].name, "Stardust Dragon");
}

BOOST_AUTO_TEST_CASE(DeleteCard)
{
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Castor");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Pot of Duality");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Abyss Dweller");

    auto cards = deckSet->cards();
    auto main = cards[ygo::data::DeckType::MAIN];
    auto side = cards[ygo::data::DeckType::SIDE];
    auto extra = cards[ygo::data::DeckType::EXTRA];
    BOOST_CHECK_EQUAL(main.size(), 1);
    BOOST_CHECK_EQUAL(side.size(), 1);
    BOOST_CHECK_EQUAL(extra.size(), 1);

    deckSet->deleteCard(ygo::data::DeckType::MAIN, "Evilswarm Castor");
    deckSet->deleteCard(ygo::data::DeckType::SIDE, "Pot of Duality");
    deckSet->deleteCard(ygo::data::DeckType::EXTRA, "Abyss Dweller");

    cards = deckSet->cards();
    main = cards[ygo::data::DeckType::MAIN];
    side = cards[ygo::data::DeckType::SIDE];
    extra = cards[ygo::data::DeckType::EXTRA];
    BOOST_CHECK_EQUAL(main.size(), 0);
    BOOST_CHECK_EQUAL(side.size(), 0);
    BOOST_CHECK_EQUAL(extra.size(), 0);
}

BOOST_AUTO_TEST_CASE(DeleteNotPresentCard)
{
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Castor");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Pot of Duality");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Abyss Dweller");

    auto cards = deckSet->cards();
    auto main = cards[ygo::data::DeckType::MAIN];
    auto side = cards[ygo::data::DeckType::SIDE];
    auto extra = cards[ygo::data::DeckType::EXTRA];
    BOOST_CHECK_EQUAL(main.size(), 1);
    BOOST_CHECK_EQUAL(side.size(), 1);
    BOOST_CHECK_EQUAL(extra.size(), 1);

    deckSet->deleteCard(ygo::data::DeckType::MAIN, "Blue-Eyes White Dragon");
    deckSet->deleteCard(ygo::data::DeckType::SIDE, "Pot of Greed");
    deckSet->deleteCard(ygo::data::DeckType::EXTRA, "Gagaga Cowboy");

    cards = deckSet->cards();
    main = cards[ygo::data::DeckType::MAIN];
    side = cards[ygo::data::DeckType::SIDE];
    extra = cards[ygo::data::DeckType::EXTRA];
    BOOST_CHECK_EQUAL(main.size(), 1);
    BOOST_CHECK_EQUAL(side.size(), 1);
    BOOST_CHECK_EQUAL(extra.size(), 1);
}

BOOST_AUTO_TEST_CASE(Validate)
{
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Castor");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Castor");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Castor");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Heliotrope");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Heliotrope");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Heliotrope");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Mandragora");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Mandragora");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Evilswarm Mandragora");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Gravekeeper's Commandant");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Gravekeeper's Commandant");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Gravekeeper's Descendant");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Gravekeeper's Spy");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Gravekeeper's Spy");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Gravekeeper's Spy");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Rescue Rabbit");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Thunder King Rai-Oh");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Allure of Darkness");

    BOOST_CHECK_EQUAL(deckSet->validate(), false);

    deckSet->addCard(ygo::data::DeckType::MAIN, "Dark Hole");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infestation Pandemic");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infestation Pandemic");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Necrovalley");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Necrovalley");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Necrovalley");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Pot of Duality");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Pot of Duality");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Reinforcement of the Army");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Reinforcement of the Army");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Bottomless Trap Hole");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Compulsory Evacuation Device");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Dimensional Prison");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Dimensional Prison");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Dimensional Prison");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Fiendish Chain");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Fiendish Chain");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infestation Infection");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Solemn Warning");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Torrential Tribute");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Wiretap");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Wiretap");

    BOOST_CHECK_EQUAL(deckSet->validate(), true);

    deckSet->addCard(ygo::data::DeckType::EXTRA, "Abyss Dweller");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Cairngorgon, Antiluminescent Knight");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Evilswarm Bahamut");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Evilswarm Exciton Knight");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Evilswarm Ophion");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Evilswarm Ophion");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Evilswarm Ouroboros");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Evilswarm Thanatos");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Gagaga Cowboy");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Maestroke the Symphony Djinn");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Number 101: Silent Honor ARK");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Number 101: Silent Honor ARK");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Number 103: Ragnazero");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Number 66: Master Key Beetle");
    deckSet->addCard(ygo::data::DeckType::EXTRA, "Number 82: Heartlandraco");

    BOOST_CHECK_EQUAL(deckSet->validate(), true);

    deckSet->addCard(ygo::data::DeckType::SIDE, "Trap Hole");
    deckSet->addCard(ygo::data::DeckType::SIDE, "White Hole");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Debunk");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Debunk");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Mirror Force");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Mirror Force");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Mirror Force");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Evilswarm Mandragora");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Mind Control");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Soul Release");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Spiritualism");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Spiritualism");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Vanity's Emptiness");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Vanity's Emptiness");
    deckSet->addCard(ygo::data::DeckType::SIDE, "Vanity's Emptiness");

    BOOST_CHECK_EQUAL(deckSet->validate(), true);

    deckSet->addCard(ygo::data::DeckType::MAIN, "Archfiend Heiress");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Armageddon Knight");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Dark Grepher");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Dark Grepher");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Archfiend");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Archfiend");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Archfiend");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Necromancer");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Necromancer");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Stygian Street Patrol");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Stygian Street Patrol");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Stygian Street Patrol");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Summoner Monk");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Summoner Monk");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Barrier");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Break");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Break");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Infernity Break");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Trap Stun");
    deckSet->addCard(ygo::data::DeckType::MAIN, "Trap Stun");

    BOOST_CHECK_EQUAL(deckSet->validate(), true);
}

BOOST_AUTO_TEST_SUITE_END()
