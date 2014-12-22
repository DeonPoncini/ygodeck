---
Title: ygodeck
Description: YuGiOh! Deck Builder Library
Author: Deon Poncini

---
ygodeck
===============

Developed by Deon Poncini <dex1337@gmail.com>

Description
-----------
This is a deck building library for the YuGiOh! card game. The idea is that this
library can be used for a multitude of applications that want to have a YuGiOh!
deck builder. It just requires an application frontend. An example application
front end written in Qt is availabe in the test folder.

This library will generate a database file with all the available cards, and
allow users to search the cards and build decks associated with users.

Building
--------
The test program depends on [qt5](http://qt-project.org/qt5). The main library
depends on a series of other libraries I have written. To build the source code,
check out the documentation at [scripts](http://github.com/DeonPoncini/scripts).

When you have checked out the scripts project, you will need to define a repo
manifest for your project. You will need to start with the following skeleton:


    <manifest>
        <remote name="dp" fetch="git@github.com:DeonPoncini" />
        <default revision="refs/heads/master" remote="dp" sync-j="4" />
        <project name="mindbw" remote="dp" path="mindbw" />
        <project name="ygodata" remote="dp" path="ygodata" />
        <project name="ygodeck" remote="dp" path="ygodeck" />
        <project name="zephyr" remote="dp" path="zephyr" />
    </manifest>

This will check out the basic libraries required to build the deck builder.
If you do a build at this point, it will generate the example application along
with the set of .so files needed to link your application against.

If you wish to write a client program, simply extend the manifest like so:

    <manifest>
        <remote name="dp" fetch="git@github.com:DeonPoncini" />
        <remote name="your-remote" fetch="your-remote-path" />
        <default revision="refs/heads/master" remote="dp" sync-j="4" />
        <default revision="refs/heads/master" remote="your-remote" sync-j="4" />
        <project name="mindbw" remote="dp" path="mindbw" />
        <project name="ygodata" remote="dp" path="ygodata" />
        <project name="ygodeck" remote="dp" path="ygodeck" />
        <project name="zephyr" remote="dp" path="zephyr" />
        <project name="application" remote="your-remote" path="application" />
    </manifest>

Where application links against the ygodeck library by writing in your
CMakeLists.txt

    find_package(ygodeck)
    include_directories(${YGODECK_INCLUDE_DIRS})
    target_link_libraries(application ${YGODECK_LIBRARIES})

If you don't want to build your application within the scripts build system,
feel free to package the libraries and include files as you wish to link against
your application

Usage
-----
There are several stages to generating the card data needed by the database.
Pre generated data for each card is available in the card/ directory.
Information about the cards is currently scraped from the YuGiOh! wikia using
the script scrape-cards in the scripts/ directory. This script will only
scrape card data if the card data file doesn't exist in the card/ directory,
so if you want to rescrape the data delete the card file. The scrape-formats
script will scrape the banlist and illegal card lists to determine what cards
are legal for each banlist, and output this information to the format/ directory.

Once this data has been scraped, (which you should not need to do, but useful
if you want to get the latest data and this library hasn't been updated with it
yet) the first thing you need to do is run the load-db script. This will create
a database file called card.db inside the ygodeck build directory. This is an
sqlite3 database file that will be used to back the deck builder library.
load-db will use the schema present under the db/ folder to create tables in the
database, then will load all the card and format information under card and
format directories.

To examine how to use the library, it is instructive to go through the example
application. The only important files are Lobby.cpp and DeckBuilder.cpp, the
rest of the files are Qt5 UI files to show the application GUI and can be safely
ignored.

First we start with Lobby.cpp. This is where the user has to select the card.db
file, as well as Add a new user, Remove an existing user or Login with an
existing user. ygodeck has the concept of Users, each user has a collection of
deck sets stored under their name. A deck set is a triple of main, side and
extra that make up a deck. So any applicatiion that uses the library needs a way
to create a user. This is done by constructing a ygo::deck::User object with the
second parameter set to true, which means create user. This will throw an
exception if the user doesn't exist. The Lobby::OnAddUser() function shows how
to achieve this. To use a user, simply create a User object without the second
parameter, just a username, and it will use a user already in the database.

Before doing anything however, the application needs to tell the library where
to find the card.db file, or any sqlite3 database file you wish to use as the
card searching database. This is done by calling

    ygo::deck::DB::get().path(filepath);

This needs to be done before any calls the any other function, or they will
terminate as there is no database to open. This is done in the
Lobby::onSelectDB() function.

Now we can move our attention to DeckBuilder.cpp. The first thing a user would
want to do is create a DeckSet. A DeckSet is a set of three decks, main, side
and extra. To create a DeckSet, the user needs a format object to specify what
format the user wants to play in. The format takes two parameters, whether the
user wants to play Traditional or Advanced, and what date the format is.

    ygo::deck::Format f(ygo::data::Format::ADVANCED, "April 2014 TCG");

The Format string must match a valid format as found in the format/ folder.
Now we can create a DeckSet like so:

    ygo::deck::DeckSet ds("name", user, f, true);

The parameters are name of the deck set, the user to associate this deck set
with, the deckset format, and the fourth parameter true indicates create a new
deck set. If this was false or missing it would open an existing deck set
instead. An example of creating a new deck set is at DeckBuilder::onAddDeckSet()

Now we have a deck set, we want to search for cards. To do this, we create a
CardSelector object, and run queries on it. CardSelector can search on all card
properties, and name searches support partial matches. An example query would
be like:

    ygo::Deck::CardSelector cs;
    auto result = cs.name("Black").cardType(ygo::data::CardType::MONSTER).
        level(7, mindbw::Operator::GT).execute();

Result will be a vector of strings containing all card names of Monster cards,
of level 7 or greater with the word "Black" in their name. A simple name only
search is implemented in the example application in DeckBuilder::onSearchCard().

Now you have a list of cards, you want to be able to actually get the card info
associated with the card. The CardSelector can return the actuall card info for
you as well using the query function:

    ygo::Deck::CardSelector cs;
    auto info = cs.query("Jinzo");

Info will be of type ygo::data::StaticCardData, a struct containing all the
various data that is printed on a card (hence static static). An example of
using this data and reading it out is in the DeckBuilder::cardInfo() function.
Note that the name given to the query function needs to be an exact match to
an existing card name in the database.

Now we have a way of searching cards we want to add them to our DeckSet. This
is achieved through the ygo::deck::DeckSet::addCard function. This takes the
deck type to add it to (main, side, extra) and a string name of the card. This
name must exactly match a card in the database. This function can return a few
different errors:
* LIMIT\_REACHED trying to add more copies than allowed by the banlist
* DECK\_FULL the deck is at capacity
* FORBIDDEN card is forbidden in this format
* OK card was successfully added

The final step is to allow the user to export their deck so it can be used in
an application that plays with YuGiOh! decks, or displayed on a webpage, etc.
First, the deck needs to be validated that it is legal for the format. This can
be achieved through calling the ygo::deck::DeckSet::validate() function, which
returns false if the deck is invalid (such as not enough cards). To export the
function to a JSON string, use:

    auto serial = ygo::data::fromCardMap(deckSet.cards())

This will convert the CardMap representation of the DeckSet into a JSON string
which can be used in other applications. See DeckBuilder::onExport() for an
example of this.

License
-------
Copyright (c) 2014 Deon Poncini.
See the LICENSE file for license rights and limitations (MIT)
