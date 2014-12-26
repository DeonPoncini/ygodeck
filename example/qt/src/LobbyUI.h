#ifndef LOBBYUI_H
#define LOBBYUI_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>

class LobbyUI
{
public:
    QWidget* centralWidget;

    QGridLayout* gridLayout;
    QHBoxLayout* controlsLayout;
    QHBoxLayout* fileBrowseLayout;
    QWidget* controlsWidget;
    QPushButton* login;
    QPushButton* addUser;
    QPushButton* removeUser;
    QLineEdit* username;
    QWidget* fileBrowseWidget;
    QLabel* fileLabel;
    QLineEdit* dbFile;
    QPushButton* fileBrowse;

    void setupUi(QMainWindow *mainWindow);
};

#endif
