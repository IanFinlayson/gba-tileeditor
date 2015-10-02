#include <iostream>

#include "ui_mainwindow.h"

int main(int argc, char** argv) {
    /* pass flags to QT */
    QApplication app(argc, argv);

    /* load the ui from the one QT generates from the ui file */
    QMainWindow* widget = new QMainWindow;
    Ui_MainWindow ui;
    ui.setupUi(widget);
    ui.actionNew->setIcon(QIcon(":/icons/new.png"));
    ui.actionOpen->setIcon(QIcon(":/icons/open.png"));
    ui.actionSave->setIcon(QIcon(":/icons/save.png"));
    ui.actionSave_As->setIcon(QIcon(":/icons/save-as.png"));
    ui.actionQuit->setIcon(QIcon(":/icons/quit.png"));

    ui.actionCopy->setIcon(QIcon(":/icons/copy.png"));
    ui.actionCut->setIcon(QIcon(":/icons/cut.png"));
    ui.actionPaste->setIcon(QIcon(":/icons/paste.png"));
    ui.actionUndo->setIcon(QIcon(":/icons/undo.png"));
    ui.actionRedo->setIcon(QIcon(":/icons/redo.png"));

    ui.actionShow_Grid->setIcon(QIcon(":/icons/show-grid.png"));
    ui.actionZoom_In->setIcon(QIcon(":/icons/zoom-in.png"));
    ui.actionZoom_Out->setIcon(QIcon(":/icons/zoom-out.png"));

    /* set up the graphics area */
    QGraphicsScene* scene = new QGraphicsScene(widget);
    ui.graphicsView->setScene(scene);

    /* just a test */
    scene->addText("HELLO", QFont("Arial", 20));


    /* show the window and start the program */
    widget->show();
    return app.exec();
}

