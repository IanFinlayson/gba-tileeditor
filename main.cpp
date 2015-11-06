/* main.cpp
 * main function for the tile editor GUI */

#include "main.h"
#include "ui_mainwindow.h"

/* functions for the EditorWindow object which just contains callbacks
 * for every action */

EditorWindow::EditorWindow(QApplication* app) {
    this->app = app;
}

/* set up all the signal and slot triggers for each action */
void EditorWindow::setup_triggers(Ui_MainWindow* ui) {
    QObject::connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));


}

/* called when the quit action is taken */
void EditorWindow::close( ) {
    app->exit( );
}






int main(int argc, char** argv) {
    /* pass flags to QT */
    QApplication app(argc, argv);

    /* load the ui from the one QT generates from the ui file */
    EditorWindow* window = new EditorWindow(&app);
    Ui_MainWindow ui;
    ui.setupUi(window);
    ui.actionNew->setIcon(QIcon(":/icons/new.png"));
    ui.actionOpen->setIcon(QIcon(":/icons/open.png"));
    ui.actionSave->setIcon(QIcon(":/icons/save.png"));
    ui.actionSave_As->setIcon(QIcon(":/icons/save-as.png"));
    ui.actionSet_Image->setIcon(QIcon(":/icons/set-image.png")); 
    ui.actionQuit->setIcon(QIcon(":/icons/quit.png"));

    ui.actionCopy->setIcon(QIcon(":/icons/copy.png"));
    ui.actionCut->setIcon(QIcon(":/icons/cut.png"));
    ui.actionPaste->setIcon(QIcon(":/icons/paste.png"));
    ui.actionUndo->setIcon(QIcon(":/icons/undo.png"));
    ui.actionRedo->setIcon(QIcon(":/icons/redo.png"));

    ui.actionShow_Grid->setIcon(QIcon(":/icons/show-grid.png"));
    ui.actionZoom_In->setIcon(QIcon(":/icons/zoom-in.png"));
    ui.actionZoom_Out->setIcon(QIcon(":/icons/zoom-out.png"));

    /* set up the graphics areas */
    QGraphicsScene* map = new QGraphicsScene(window);
    QGraphicsScene* palette = new QGraphicsScene(window);
    ui.map_view->setScene(map);
    ui.palette_view->setScene(palette);

    /* just a test */
    map->addText("MAP", QFont("Arial", 20));
    palette->addText("PALETTE", QFont("Arial", 20));

    /* show the window and start the program */
    window->show();
    return app.exec();
}

