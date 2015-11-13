/* main.cpp
 * main function for the tile editor GUI */

#include <QMessageBox>
#include <QFileDialog>
#include "main.h"
#include "ui_mainwindow.h"

/* functions for the EditorWindow object which just contains callbacks
 * for every action */

EditorWindow::EditorWindow(QApplication* app) {
    this->app = app;
}

void EditorWindow::setAreas( QGraphicsScene* map, QGraphicsScene* palette) {
    this->map = map;
    this->palette = palette;
}

/* set up all the signal and slot triggers for each action */
void EditorWindow::setup_triggers(Ui_MainWindow* ui) {
    QObject::connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(on_new()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(on_open()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_save()));
    QObject::connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(on_save_as()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(on_quit()));
    QObject::connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(on_undo()));
    QObject::connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(on_redo()));
    QObject::connect(ui->actionShow_Grid, SIGNAL(triggered()), this, SLOT(on_grid()));
    QObject::connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(on_cut()));
    QObject::connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(on_copy()));
    QObject::connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(on_paste()));
    QObject::connect(ui->actionZoom_In, SIGNAL(triggered()), this, SLOT(on_zoom_in()));
    QObject::connect(ui->actionZoom_Out, SIGNAL(triggered()), this, SLOT(on_zoom_out()));
    QObject::connect(ui->actionSet_Image, SIGNAL(triggered()), this, SLOT(on_set_image()));
}

/* called when the various actions are taken */
void EditorWindow::on_new( ) {
    QMessageBox msgBox;
    msgBox.setText("New is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_open( ) {
    QMessageBox msgBox;
    msgBox.setText("Open is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_save( ) {
    QMessageBox msgBox;
    msgBox.setText("Save is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_save_as( ) {
    QMessageBox msgBox;
    msgBox.setText("Save as is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_set_image( ) {
    /* get the file name of the file they wish to use */
    QString file = QFileDialog::getOpenFileName(this,
         tr("Set Image"), "", tr("Image Files (*.png)"));

    /* check if the string is null */
    if (file.isNull( )) {
        return;
    }

    /* set the image itself */
    tiles.load(file);

    /* display it in the bottom area */
    QPixmap p = QPixmap::fromImage(tiles);
    palette->addPixmap(p);
}

void EditorWindow::on_quit( ) {
    app->exit( );
}

void EditorWindow::on_undo( ) {
    QMessageBox msgBox;
    msgBox.setText("Undo is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_redo( ) {
    QMessageBox msgBox;
    msgBox.setText("Redo is not implemented yet :(");
    msgBox.exec( );
}

void EditorWindow::on_cut( ) {
    QMessageBox msgBox;
    msgBox.setText("Cut is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_copy( ) {
    QMessageBox msgBox;
    msgBox.setText("Copy is not implemented yet :(");
    msgBox.exec( ); 
}

void EditorWindow::on_paste( ) {
    QMessageBox msgBox;
    msgBox.setText("Paste is not implemented yet :(");
    msgBox.exec( );
}

void EditorWindow::on_grid( ) {
    QMessageBox msgBox;
    msgBox.setText("Grid is not implemented yet :(");
    msgBox.exec( );
}

void EditorWindow::on_zoom_in( ) {
    QMessageBox msgBox;
    msgBox.setText("Zoom is not implemented yet :(");
    msgBox.exec( );
}

void EditorWindow::on_zoom_out( ) {
    QMessageBox msgBox;
    msgBox.setText("Zoom is not implemented yet :(");
    msgBox.exec( );
}




/* main entry point */
int main(int argc, char** argv) {
    /* pass flags to QT */
    QApplication app(argc, argv);

    /* load the ui from the one QT generates from the ui file */ 
    EditorWindow* window = new EditorWindow(&app);
    Ui_MainWindow ui;
    ui.setupUi(window);
    window->setup_triggers(&ui);

    /* set up the graphics areas */ 
    QGraphicsScene* map = new QGraphicsScene(window);
    QGraphicsScene* palette = new QGraphicsScene(window);
    window->setAreas(map, palette);

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

    ui.map_view->setScene(map);
    ui.palette_view->setScene(palette);

    /* just a test */
    map->addText("MAP", QFont("Arial", 20));

    /* show the window and start the program */
    window->show();
    return app.exec();
}

