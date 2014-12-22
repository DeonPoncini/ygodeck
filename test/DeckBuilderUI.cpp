#include "DeckBuilderUI.h"

#include <QApplication>

void DeckBuilderUI::setupUi(QMainWindow* mainWindow)
{
    if (mainWindow->objectName().isEmpty())
        mainWindow->setObjectName(QStringLiteral("DeckBuilderUI"));
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mainWindow->sizePolicy().hasHeightForWidth());
    mainWindow->setSizePolicy(sizePolicy);
    centralWidget = new QWidget(mainWindow);
    gridWidget = new QWidget(centralWidget);
    gridLayout = new QGridLayout;
    gridWidget->setLayout(gridLayout);

    // deck set controls
    deckSetWidget = new QWidget;
    deckSetLayout = new QVBoxLayout;
    deckSetHeading = new QLabel("Choose a deck set");
    deckSets = new QListWidget;
    deckSetControlLayout = new QHBoxLayout;
    deckSetFormatLayout = new QHBoxLayout;
    addDeckSet = new QPushButton("Add");
    removeDeckSet = new QPushButton("Delete");
    addDeckSetName = new QLineEdit;
    deckSetFormats = new QComboBox;
    deckSetFormatDates = new QComboBox;
    deckSetControlLayout->addWidget(addDeckSetName);
    deckSetControlLayout->addWidget(addDeckSet);
    deckSetControlLayout->addWidget(removeDeckSet);
    deckSetFormatLayout->addWidget(deckSetFormats);
    deckSetFormatLayout->addWidget(deckSetFormatDates);
    deckSetLayout->addWidget(deckSetHeading);
    deckSetLayout->addWidget(deckSets);
    deckSetLayout->addLayout(deckSetControlLayout);
    deckSetLayout->addLayout(deckSetFormatLayout);
    deckSetWidget->setLayout(deckSetLayout);

    // main deck controls
    mainDeckWidget = new QWidget;
    mainDeckContainer = new QHBoxLayout;
    mainDeckLayout = new QVBoxLayout;
    mainDeckTitleLayout = new QHBoxLayout;
    mainDeckTitle = new QLabel("Main");
    mainDeckCount = new QLabel;
    mainDeck = new QListWidget;
    mainDeckControlsLayout = new QVBoxLayout;
    addToMainDeck = new QPushButton("Add");
    removeFromMainDeck = new QPushButton("Delete");
    mainDeckTitleLayout->addWidget(mainDeckTitle);
    mainDeckTitleLayout->addWidget(mainDeckCount);
    mainDeckLayout->addLayout(mainDeckTitleLayout);
    mainDeckLayout->addWidget(mainDeck);
    mainDeckControlsLayout->addWidget(addToMainDeck);
    mainDeckControlsLayout->addWidget(removeFromMainDeck);
    mainDeckContainer->addLayout(mainDeckLayout);
    mainDeckContainer->addLayout(mainDeckControlsLayout);
    mainDeckWidget->setLayout(mainDeckContainer);

    // side deck controls
    sideDeckWidget = new QWidget;
    sideDeckContainer = new QHBoxLayout;
    sideDeckLayout = new QVBoxLayout;
    sideDeckTitleLayout = new QHBoxLayout;
    sideDeckTitle = new QLabel("Side");
    sideDeckCount = new QLabel;
    sideDeck = new QListWidget;
    sideDeckControlsLayout = new QVBoxLayout;
    addToSideDeck = new QPushButton("Add");
    removeFromSideDeck = new QPushButton("Delete");
    sideDeckTitleLayout->addWidget(sideDeckTitle);
    sideDeckTitleLayout->addWidget(sideDeckCount);
    sideDeckLayout->addLayout(sideDeckTitleLayout);
    sideDeckLayout->addWidget(sideDeck);
    sideDeckControlsLayout->addWidget(addToSideDeck);
    sideDeckControlsLayout->addWidget(removeFromSideDeck);
    sideDeckContainer->addLayout(sideDeckLayout);
    sideDeckContainer->addLayout(sideDeckControlsLayout);
    sideDeckWidget->setLayout(sideDeckContainer);

    // extra deck controls
    extraDeckWidget = new QWidget;
    extraDeckContainer = new QHBoxLayout;
    extraDeckLayout = new QVBoxLayout;
    extraDeckTitleLayout = new QHBoxLayout;
    extraDeckTitle = new QLabel("Extra");
    extraDeckCount = new QLabel;
    extraDeck = new QListWidget;
    extraDeckControlsLayout = new QVBoxLayout;
    addToExtraDeck = new QPushButton("Add");
    removeFromExtraDeck = new QPushButton("Delete");
    extraDeckTitleLayout->addWidget(extraDeckTitle);
    extraDeckTitleLayout->addWidget(extraDeckCount);
    extraDeckLayout->addLayout(extraDeckTitleLayout);
    extraDeckLayout->addWidget(extraDeck);
    extraDeckControlsLayout->addWidget(addToExtraDeck);
    extraDeckControlsLayout->addWidget(removeFromExtraDeck);
    extraDeckContainer->addLayout(extraDeckLayout);
    extraDeckContainer->addLayout(extraDeckControlsLayout);
    extraDeckWidget->setLayout(extraDeckContainer);

    // card searcher
    cardSearchWidget = new QWidget;
    cardSearchLayout = new QVBoxLayout;
    cardSearchTitleLayout = new QHBoxLayout;
    cardSearchBox = new QLineEdit;
    cardSearchButton = new QPushButton("Search");
    cardSearchResults = new QListWidget;
    cardSearchTitleLayout->addWidget(cardSearchBox);
    cardSearchTitleLayout->addWidget(cardSearchButton);
    cardSearchLayout->addLayout(cardSearchTitleLayout);
    cardSearchLayout->addWidget(cardSearchResults);
    cardSearchWidget->setLayout(cardSearchLayout);

    // card info
    cardInfoWidget = new QWidget;
    cardInfoLayout = new QGridLayout;
    cardNameLabel = new QLabel("Name");
    cardName = new QLabel;
    cardTypeLabel = new QLabel("Card Type");
    cardType = new QLabel;
    attributeLabel = new QLabel("Attribute");
    attribute = new QLabel;
    monsterTypeLabel = new QLabel("Monster Type");
    monsterType = new QLabel;
    typeLabel = new QLabel("Type");
    type = new QLabel;
    monsterAbilityLabel = new QLabel("Monster Ability");
    monsterAbility = new QLabel;
    levelLabel = new QLabel("Level");
    level = new QLabel;
    attackLabel = new QLabel("ATK");
    attack = new QLabel;
    defenseLabel = new QLabel("DEF");
    defense = new QLabel;
    lpendulumLabel = new QLabel("L Pendulum");
    lpendulum = new QLabel;
    rpendulumLabel = new QLabel("R Pendulum");
    rpendulum = new QLabel;
    spellTypeLabel = new QLabel("Spell Type");
    spellType = new QLabel;
    trapTypeLabel = new QLabel("Trap Type");
    trapType = new QLabel;
    textLabel = new QLabel("Text");
    text = new QLabel;
    cardInfoLayout->addWidget(cardNameLabel,0,0);
    cardInfoLayout->addWidget(cardName,0,1);
    cardInfoLayout->addWidget(cardTypeLabel,1,0);
    cardInfoLayout->addWidget(cardType,1,1);
    cardInfoLayout->addWidget(attributeLabel,2,0);
    cardInfoLayout->addWidget(attribute,2,1);
    cardInfoLayout->addWidget(monsterTypeLabel,3,0);
    cardInfoLayout->addWidget(monsterType,3,1);
    cardInfoLayout->addWidget(typeLabel,4,0);
    cardInfoLayout->addWidget(type,4,1);
    cardInfoLayout->addWidget(monsterAbilityLabel,5,0);
    cardInfoLayout->addWidget(monsterAbility,5,1);
    cardInfoLayout->addWidget(levelLabel,6,0);
    cardInfoLayout->addWidget(level,6,1);
    cardInfoLayout->addWidget(attackLabel,7,0);
    cardInfoLayout->addWidget(attack,7,1);
    cardInfoLayout->addWidget(defenseLabel,8,0);
    cardInfoLayout->addWidget(defense,8,1);
    cardInfoLayout->addWidget(lpendulumLabel,9,0);
    cardInfoLayout->addWidget(lpendulum,9,1);
    cardInfoLayout->addWidget(rpendulumLabel,10,0);
    cardInfoLayout->addWidget(rpendulum,10,1);
    cardInfoLayout->addWidget(spellTypeLabel,11,0);
    cardInfoLayout->addWidget(spellType,11,1);
    cardInfoLayout->addWidget(trapTypeLabel,12,0);
    cardInfoLayout->addWidget(trapType,12,1);
    cardInfoLayout->addWidget(textLabel,13,0);
    cardInfoLayout->addWidget(text,13,1);
    cardInfoWidget->setLayout(cardInfoLayout);

    // game controls
    playButton = new QPushButton("Play");

    gridLayout->addWidget(deckSetWidget,0,0);
    gridLayout->addWidget(mainDeckWidget,0,1);
    gridLayout->addWidget(sideDeckWidget,1,1);
    gridLayout->addWidget(extraDeckWidget,2,1);
    gridLayout->addWidget(cardSearchWidget,0,3);
    gridLayout->addWidget(deckSetWidget,0,0);
    gridLayout->addWidget(cardInfoWidget,1,0);
    gridLayout->addWidget(playButton,0,4);

    mainWindow->setCentralWidget(centralWidget);
    mainWindow->setWindowTitle(QApplication::translate("DeckBuilder",
                "DeckBuilder",0));
}
