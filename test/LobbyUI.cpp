#include "LobbyUI.h"

#include <QApplication>

void LobbyUI::setupUi(QMainWindow *mainWindow)
{
    if (mainWindow->objectName().isEmpty())
        mainWindow->setObjectName(QStringLiteral("LobbyUI"));
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mainWindow->sizePolicy().hasHeightForWidth());
    mainWindow->setSizePolicy(sizePolicy);
    centralWidget = new QWidget(mainWindow);

    gridLayout = new QGridLayout;

    controlsLayout = new QHBoxLayout;
    controlsWidget = new QWidget;
    login = new QPushButton("Login");
    addUser = new QPushButton("Add");
    removeUser = new QPushButton("Remove");
    username = new QLineEdit;

    fileBrowseLayout = new QHBoxLayout;
    fileBrowseWidget = new QWidget;
    fileLabel = new QLabel("DB File");
    fileBrowse = new QPushButton("Browse");
    dbFile = new QLineEdit;

    controlsLayout->addWidget(login);
    controlsLayout->addWidget(addUser);
    controlsLayout->addWidget(removeUser);
    controlsWidget->setLayout(controlsLayout);

    fileBrowseLayout->addWidget(fileLabel);
    fileBrowseLayout->addWidget(dbFile);
    fileBrowseLayout->addWidget(fileBrowse);
    fileBrowseWidget->setLayout(fileBrowseLayout);

    gridLayout->addWidget(controlsWidget,0,0);
    gridLayout->addWidget(username,1,0);
    gridLayout->addWidget(fileBrowseWidget,2,0);

    centralWidget->setLayout(gridLayout);
    mainWindow->setCentralWidget(centralWidget);
    mainWindow->setWindowTitle(QApplication::translate("Lobby", "Lobby", 0));
}
