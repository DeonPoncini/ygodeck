#include "DeckBuilder.h"

#include <ygo/data/Serialize.h>
#include <ygo/deck/CardSelector.h>
#include <ygo/deck/Format.h>

#include <QMessageBox>

#include <utility>

DeckBuilder::DeckBuilder(ygo::deck::User user, QWidget *parent) :
    QMainWindow(parent),
    mUser(std::move(user))
{
    ui.setupUi(this);

    wireControls();
}

void DeckBuilder::start()
{
    showMaximized();

    // fill in the deck sets
    auto decks = mUser.deckSets();
    for (auto&& d : decks) {
        ui.deckSets->addItem(QString::fromStdString(d.name()));
        mDeckSets.insert({d.name(),d});
    }

    // fill in the combo boxes
    auto formats = ygo::deck::Format::formats();
    for (auto&& f : formats) {
        ui.deckSetFormats->addItem(QString::fromStdString(f));
    }
    auto dates = ygo::deck::Format::formatDates();
    for (auto&& d : dates) {
        ui.deckSetFormatDates->addItem(QString::fromStdString(d));
    }
}

void DeckBuilder::onAddDeckSet()
{
    // add the deck set
    auto format = ui.deckSetFormats->currentText().toStdString();
    auto formatDate = ui.deckSetFormatDates->currentText().toStdString();
    auto name = ui.addDeckSetName->text().toStdString();

    ygo::deck::Format f(ygo::data::toFormat(format), formatDate);
    ygo::deck::DeckSet ds(name, mUser, f, true);
    if (ds.cards()[ygo::data::DeckType::MAIN].empty() &&
        ds.cards()[ygo::data::DeckType::SIDE].empty() &&
        ds.cards()[ygo::data::DeckType::EXTRA].empty()) {
        ui.deckSets->addItem(QString::fromStdString(ds.name()));
        mDeckSets.insert({ds.name(),ds});
    }
}

void DeckBuilder::onRemoveDeckSet()
{
    auto item = ui.deckSets->takeItem(ui.deckSets->currentRow());
    auto deckSet = mDeckSets.find(item->text().toStdString())->second;
    auto name = deckSet.name();
    deckSet.remove();
    mDeckSets.erase(name);
    delete item;
}

void DeckBuilder::onDeckSetSelected(QListWidgetItem* item)
{
    if (item == nullptr) return;
    auto it = mDeckSets.find(item->text().toStdString());
    if (it == mDeckSets.end()) return;
    auto deckSet = it->second;
    auto cards = deckSet.cards();
    // fill up
    auto main = *cards.find(ygo::data::DeckType::MAIN);
    ui.mainDeck->clear();
    for (auto&& c : main.second) {
        ui.mainDeck->addItem(QString::fromStdString(c.name));
    }

    auto side = *cards.find(ygo::data::DeckType::SIDE);
    ui.sideDeck->clear();
    for (auto&& c : side.second) {
        ui.sideDeck->addItem(QString::fromStdString(c.name));
    }

    auto extra = *cards.find(ygo::data::DeckType::EXTRA);
    ui.extraDeck->clear();
    for (auto&& c : extra.second) {
        ui.extraDeck->addItem(QString::fromStdString(c.name));
    }

    // select the correct format parameters
    auto format = deckSet.format();
    ui.deckSetFormats->setCurrentIndex(
            ui.deckSetFormats->findText(QString::fromStdString(
                    ygo::data::fromFormat(format.format()))));
    ui.deckSetFormatDates->setCurrentIndex(
            ui.deckSetFormatDates->findText(QString::fromStdString(
                    format.formatDate())));

    updateDeckCounts();
}

void DeckBuilder::onSearchCard()
{
    ygo::deck::CardSelector cs;
    auto results = cs.name(ui.cardSearchBox->text().toStdString()).execute();
    ui.cardSearchResults->clear();
    for (auto&& c : results) {
        ui.cardSearchResults->addItem(QString::fromStdString(c));
    }

}

void DeckBuilder::onAddMain()
{
    onAdd(ygo::data::DeckType::MAIN, ui.mainDeck);
}

void DeckBuilder::onRemoveMain()
{
    onRemove(ygo::data::DeckType::MAIN, ui.mainDeck);
}

void DeckBuilder::onAddSide()
{
    onAdd(ygo::data::DeckType::SIDE, ui.sideDeck);
}

void DeckBuilder::onRemoveSide()
{
    onRemove(ygo::data::DeckType::SIDE, ui.sideDeck);
}

void DeckBuilder::onAddExtra()
{
    onAdd(ygo::data::DeckType::EXTRA, ui.extraDeck);
}

void DeckBuilder::onRemoveExtra()
{
    onRemove(ygo::data::DeckType::EXTRA, ui.extraDeck);
}

void DeckBuilder::onMainSelected(QListWidgetItem* item)
{
    if (item == nullptr) return;
    cardInfo(item->text().toStdString());
}

void DeckBuilder::onSideSelected(QListWidgetItem* item)
{
    if (item == nullptr) return;
    cardInfo(item->text().toStdString());
}

void DeckBuilder::onExtraSelected(QListWidgetItem* item)
{
    if (item == nullptr) return;
    cardInfo(item->text().toStdString());
}

void DeckBuilder::onResultsSelected(QListWidgetItem* item)
{
    if (item == nullptr) return;
    cardInfo(item->text().toStdString());
}

void DeckBuilder::onPlay()
{
    auto item = ui.deckSets->currentItem();
    if (item == nullptr) return;
    auto deckSet = mDeckSets.find(item->text().toStdString())->second;
    if (!deckSet.validate()) {
        QMessageBox::critical(ui.centralWidget, "Deck",
                "Deck is not valid for play");
        return;
    }
    // start up the connection screen
}

void DeckBuilder::onAdd(ygo::data::DeckType deckType, QListWidget* list)
{
    auto item = ui.deckSets->currentItem();
    if (item == nullptr) return;
    auto search = ui.cardSearchResults->currentItem();
    if (search == nullptr) return;
    auto card = search->text().toStdString();
    auto deckSet = mDeckSets.find(item->text().toStdString())->second;
    auto error = deckSet.addCard(deckType, card);
    switch (error) {
        case ygo::deck::DeckError::LIMIT_REACHED:
            QMessageBox::critical(ui.centralWidget, "Deck",
                "Deck limit reached for " + QString::fromStdString(card));
            break;
        case ygo::deck::DeckError::DECK_FULL:
            QMessageBox::critical(ui.centralWidget, "Deck",
                "Deck is at capacity");
            break;
        case ygo::deck::DeckError::FORBIDDEN:
            QMessageBox::critical(ui.centralWidget, "Deck",
                "Card is forbidden " + QString::fromStdString(card));
            break;
        case ygo::deck::DeckError::OK:
            list->addItem(QString::fromStdString(card));
            updateDeckCounts();
            break;
    }
}

void DeckBuilder::onRemove(ygo::data::DeckType deckType, QListWidget* list)
{
    auto item = ui.deckSets->currentItem();
    if (item == nullptr) return;
    auto selected = list->takeItem(list->currentRow());
    if (selected == nullptr) return;
    auto card = selected->text().toStdString();
    auto deckSet = mDeckSets.find(item->text().toStdString())->second;
    deckSet.deleteCard(deckType, card);
    delete selected;
    updateDeckCounts();
}

void DeckBuilder::updateDeckCounts()
{
    auto item = ui.deckSets->currentItem();
    if (item == nullptr) return;
    auto deckSet = mDeckSets.find(item->text().toStdString())->second;
    auto cards = deckSet.cards();
    auto main = *cards.find(ygo::data::DeckType::MAIN);
    ui.mainDeckCount->setText(QString::number(main.second.size()));
    auto side = *cards.find(ygo::data::DeckType::SIDE);
    ui.sideDeckCount->setText(QString::number(side.second.size()));
    auto extra = *cards.find(ygo::data::DeckType::EXTRA);
    ui.extraDeckCount->setText(QString::number(extra.second.size()));
}

void DeckBuilder::cardInfo(const std::string& card)
{
    ygo::deck::CardSelector cs;
    auto info = cs.query(card);
    ui.cardName->setText(QString::fromStdString(info.name));
    ui.cardType->setText(QString::fromStdString(ygo::data::fromCardType(
                    info.cardType)));
    ui.attribute->setText(QString::fromStdString(ygo::data::fromAttribute(
                    info.attribute)));
    ui.monsterType->setText(QString::fromStdString(ygo::data::fromMonsterType(
                    info.monsterType)));
    ui.type->setText(QString::fromStdString(ygo::data::fromType(info.type)));
    ui.monsterAbility->setText(QString::fromStdString(
                ygo::data::fromMonsterType(info.monsterAbility)));
    ui.level->setText(QString::number(info.level));
    ui.attack->setText(QString::number(info.attack));
    ui.defense->setText(QString::number(info.defense));
    ui.lpendulum->setText(QString::number(info.lpendulum));
    ui.rpendulum->setText(QString::number(info.rpendulum));
    ui.spellType->setText(QString::fromStdString(ygo::data::fromSpellType(
                    info.spellType)));
    ui.trapType->setText(QString::fromStdString(ygo::data::fromTrapType(
                    info.trapType)));
    ui.text->setText(QString::fromStdString(info.text));
}

void DeckBuilder::wireControls()
{
    connect(ui.addDeckSet, &QPushButton::clicked, this,
            &DeckBuilder::onAddDeckSet);
    connect(ui.removeDeckSet, &QPushButton::clicked, this,
            &DeckBuilder::onRemoveDeckSet);
    connect(ui.deckSets, &QListWidget::itemClicked, this,
            &DeckBuilder::onDeckSetSelected);
    connect(ui.cardSearchButton, &QPushButton::clicked, this,
            &DeckBuilder::onSearchCard);
    connect(ui.addToMainDeck, &QPushButton::clicked, this,
            &DeckBuilder::onAddMain);
    connect(ui.removeFromMainDeck, &QPushButton::clicked, this,
            &DeckBuilder::onRemoveMain);
    connect(ui.addToSideDeck, &QPushButton::clicked, this,
            &DeckBuilder::onAddSide);
    connect(ui.removeFromSideDeck, &QPushButton::clicked, this,
            &DeckBuilder::onRemoveSide);
    connect(ui.addToExtraDeck, &QPushButton::clicked, this,
            &DeckBuilder::onAddExtra);
    connect(ui.removeFromExtraDeck, &QPushButton::clicked, this,
            &DeckBuilder::onRemoveExtra);
    connect(ui.mainDeck, &QListWidget::itemClicked, this,
            &DeckBuilder::onMainSelected);
    connect(ui.sideDeck, &QListWidget::itemClicked, this,
            &DeckBuilder::onSideSelected);
    connect(ui.extraDeck, &QListWidget::itemClicked, this,
            &DeckBuilder::onExtraSelected);
    connect(ui.cardSearchResults, &QListWidget::itemClicked, this,
            &DeckBuilder::onResultsSelected);
    connect(ui.playButton, &QPushButton::clicked, this,
            &DeckBuilder::onPlay);
}
