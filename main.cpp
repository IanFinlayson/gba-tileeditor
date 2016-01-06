/* main.cpp
 * main function for the tile editor GUI */

#include <stdio.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

/* main entry point */
int main(int argc, char** argv) {
    /* pass flags to QT */
    QApplication app(argc, argv);

    /* load the main window ui from the one QT generates from the ui file */ 
    EditorWindow* window = new EditorWindow(&app);
    Ui_MainWindow ui;
    /* ignore garbage warnings from QT */
    stderr = fopen("/tmp/gbate-error", "w");
    ui.setupUi(window);
    window->setWindowTitle("GBA Tile Editor");

    /* set up all the triggers */
    window->setup_triggers(&ui);

    /* set up the graphics areas */ 
    QGraphicsScene* map_scene = new QGraphicsScene(window);
    QGraphicsScene* palette_scene = new QGraphicsScene(window);
    window->set_areas(map_scene, palette_scene);

    /* set all of the icons */
    ui.actionNew->setIcon(QIcon(":/icons/new.png"));
    ui.actionOpen->setIcon(QIcon(":/icons/open.png"));
    ui.actionSave->setIcon(QIcon(":/icons/save.png"));
    ui.actionSave_As->setIcon(QIcon(":/icons/save-as.png"));
    ui.actionChange_Properties->setIcon(QIcon(":/icons/change-properties.png")); 
    ui.actionQuit->setIcon(QIcon(":/icons/quit.png"));
    ui.actionUndo->setIcon(QIcon(":/icons/undo.png"));
    ui.actionRedo->setIcon(QIcon(":/icons/redo.png"));
    ui.actionZoom_In->setIcon(QIcon(":/icons/zoom-in.png"));
    ui.actionZoom_Out->setIcon(QIcon(":/icons/zoom-out.png"));
    ui.actionShow_Grid->setIcon(QIcon(":/icons/show-grid.png"));

    /* set up the scenes */
    ui.map_view->setScene(map_scene);
    ui.map_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui.map_view->set_window(window);

    ui.palette_view->setScene(palette_scene);
    ui.palette_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui.palette_view->set_window(window);

    /* show the window */
    window->show();

    /* set the backgrounds to gray */
    map_scene->setBackgroundBrush(Qt::lightGray);
    palette_scene->setBackgroundBrush(Qt::lightGray);

    /* start the program */
    return app.exec();
}

