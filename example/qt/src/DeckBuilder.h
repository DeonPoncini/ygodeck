#ifndef DECKBUILDER_H
#define DECKBUILDER_H

#include "DeckBuilderUI.h"

#include <ygo/deck/User.h>
#include <ygo/deck/DeckSet.h>

#include <QMainWindow>

#include <memory>
#include <unordered_map>

class DeckBuilder : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeckBuilder(ygo::deck::User user, QWidget *parent = 0);

    void start();

private Q_SLOTS:
    void onAddDeckSet();
    void onRemoveDeckSet();
    void onDeckSetSelected(QListWidgetItem* item);
    void onSearchCard();
    void onAddMain();
    void onRemoveMain();
    void onAddSide();
    void onRemoveSide();
    void onAddExtra();
    void onRemoveExtra();
    void onMainSelected(QListWidgetItem* item);
    void onSideSelected(QListWidgetItem* item);
    void onExtraSelected(QListWidgetItem* item);
    void onResultsSelected(QListWidgetItem* item);
    void onExport();

private:
    DeckBuilderUI ui;
    ygo::deck::User mUser;

    std::unordered_map<std::string, ygo::deck::DeckSet> mDeckSets;

    void onAdd(ygo::data::DeckType deckType, QListWidget* list);
    void onRemove(ygo::data::DeckType deckType, QListWidget* list);
    void updateDeckCounts();
    void cardInfo(const std::string& card);
    void wireControls();
};

#endif
