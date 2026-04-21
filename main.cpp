#include <QApplication>
#include "presentationLayer/home/home.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Home home;
    home.show();

    return app.exec();
}
