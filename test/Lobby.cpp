#include "Lobby.h"

#include <ygo/deck/DB.h>
#include <ygo/deck/User.h>
#include <zephyr/make_unique.h>

#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

Lobby::Lobby(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    wireControls();
}

void Lobby::onLogin()
{
    if (!validateDB()) {
        return;
    }
    // try and log in
    auto username = ui.username->text().toStdString();
    try {
        ygo::deck::User user(username);
        mDeckBuilder = zephyr::make_unique<DeckBuilder>(user);
        mDeckBuilder->start();
        close();
    } catch (std::runtime_error) {
        QMessageBox::critical(ui.centralWidget, "Login",
                "User " + QString::fromStdString(username) + " not found");
    }
}

void Lobby::onAddUser()
{
    if (!validateDB()) {
        return;
    }

    auto username = ui.username->text().toStdString();
    ygo::deck::User user(username, true);
    mDeckBuilder = zephyr::make_unique<DeckBuilder>(user);
    mDeckBuilder->start();
    close();
}

void Lobby::onRemoveUser()
{
    if (!validateDB()) {
        return;
    }

    auto username = ui.username->text().toStdString();
    try {
        ygo::deck::User user(username);
        user.remove();
        QMessageBox::information(ui.centralWidget, "Login",
                "User " + QString::fromStdString(username) + " removed");
        ui.username->clear();
    } catch (std::runtime_error) {
        QMessageBox::critical(ui.centralWidget, "Login",
                "User " + QString::fromStdString(username) + " not found");
    }
}

void Lobby::onSelectDB()
{
    auto file = QFileDialog::getOpenFileName(this, tr("Select DB File"));
    ygo::deck::DB::get().path(file.toStdString());
    ui.dbFile->setText(file);
}

void Lobby::wireControls()
{
    connect(ui.login,&QPushButton::clicked,this,&Lobby::onLogin);
    connect(ui.addUser,&QPushButton::clicked,this,&Lobby::onAddUser);
    connect(ui.removeUser,&QPushButton::clicked,this,&Lobby::onRemoveUser);
    connect(ui.fileBrowse,&QPushButton::clicked,this,&Lobby::onSelectDB);
}

bool Lobby::validateDB()
{
    // check the database path is selected
    if (ygo::deck::DB::get().path().empty()) {
        QMessageBox::critical(ui.centralWidget, "Login",
                "Please select database file");
        return false;
    }
    return true;
}
