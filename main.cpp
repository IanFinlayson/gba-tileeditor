#include <iostream>

#include "ui_mainwindow.h"

int main(int argc, char** argv) {
    /* pass flags to QT */
    QApplication app(argc, argv);

    /* load the ui from the one QT generates from the ui file */
    QMainWindow* widget = new QMainWindow;
    Ui_MainWindow ui;
    ui.setupUi(widget);

    /* show the window and start the program */
    widget->show();
    return app.exec();
}

