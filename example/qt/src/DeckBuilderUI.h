#ifndef DECKBUILDERUI_H
#define DECKBUILDERUI_H

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class DeckBuilderUI
{
public:
    QWidget* centralWidget;
    QWidget* gridWidget;
    QGridLayout* gridLayout;

    // deck set controls
    QWidget* deckSetWidget;
    QVBoxLayout* deckSetLayout;
    QLabel* deckSetHeading;
    QListWidget* deckSets;
    QHBoxLayout* deckSetControlLayout;
    QHBoxLayout* deckSetFormatLayout;
    QComboBox* deckSetFormats;
    QComboBox* deckSetFormatDates;
    QPushButton* addDeckSet;
    QPushButton* removeDeckSet;
    QLineEdit* addDeckSetName;

    // main deck controls
    QWidget* mainDeckWidget;
    QHBoxLayout* mainDeckContainer;

    QVBoxLayout* mainDeckLayout;
    QHBoxLayout* mainDeckTitleLayout;
    QLabel* mainDeckTitle;
    QLabel* mainDeckCount;
    QListWidget* mainDeck;

    QVBoxLayout* mainDeckControlsLayout;
    QPushButton* addToMainDeck;
    QPushButton* removeFromMainDeck;

    // side deck controls
    QWidget* sideDeckWidget;
    QHBoxLayout* sideDeckContainer;

    QVBoxLayout* sideDeckLayout;
    QHBoxLayout* sideDeckTitleLayout;
    QLabel* sideDeckTitle;
    QLabel* sideDeckCount;
    QListWidget* sideDeck;

    QVBoxLayout* sideDeckControlsLayout;
    QPushButton* addToSideDeck;
    QPushButton* removeFromSideDeck;

    // extra deck controls
    QWidget* extraDeckWidget;
    QHBoxLayout* extraDeckContainer;

    QVBoxLayout* extraDeckLayout;
    QHBoxLayout* extraDeckTitleLayout;
    QLabel* extraDeckTitle;
    QLabel* extraDeckCount;
    QListWidget* extraDeck;

    QVBoxLayout* extraDeckControlsLayout;
    QPushButton* addToExtraDeck;
    QPushButton* removeFromExtraDeck;

    // card searcher UI
    QWidget* cardSearchWidget;
    QVBoxLayout* cardSearchLayout;
    QHBoxLayout* cardSearchTitleLayout;
    QLineEdit* cardSearchBox;
    QPushButton* cardSearchButton;
    QListWidget* cardSearchResults;

    // card info table
    QWidget* cardInfoWidget;
    QGridLayout* cardInfoLayout;
    QLabel* cardNameLabel;
    QLabel* cardName;
    QLabel* cardTypeLabel;
    QLabel* cardType;
    QLabel* attributeLabel;
    QLabel* attribute;
    QLabel* monsterTypeLabel;
    QLabel* monsterType;
    QLabel* typeLabel;
    QLabel* type;
    QLabel* monsterAbilityLabel;
    QLabel* monsterAbility;
    QLabel* levelLabel;
    QLabel* level;
    QLabel* attackLabel;
    QLabel* attack;
    QLabel* defenseLabel;
    QLabel* defense;
    QLabel* lpendulumLabel;
    QLabel* lpendulum;
    QLabel* rpendulumLabel;
    QLabel* rpendulum;
    QLabel* spellTypeLabel;
    QLabel* spellType;
    QLabel* trapTypeLabel;
    QLabel* trapType;
    QLabel* textLabel;
    QLabel* text;

    // game controls
    QPushButton* exportButton;

    void setupUi(QMainWindow* mainWindow);

};

#endif
