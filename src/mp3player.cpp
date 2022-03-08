#include <QApplication>

#include "mp3player_mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QtJack::Client client;
    Mp3PlayerMainWindow mp3PlayerMainWindow;
    mp3PlayerMainWindow.show();
    return a.exec();
}
