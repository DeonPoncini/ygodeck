#ifndef LOBBY_H
#define LOBBY_H

#include "LobbyUI.h"
#include "DeckBuilder.h"

#include <QMainWindow>
#include <memory>

class Lobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit Lobby(QWidget *parent = 0);

private Q_SLOTS:
    void onLogin();
    void onAddUser();
    void onRemoveUser();
    void onSelectDB();

private:
    LobbyUI ui;

    std::unique_ptr<DeckBuilder> mDeckBuilder;

    void wireControls();
    bool validateDB();
};

#endif
