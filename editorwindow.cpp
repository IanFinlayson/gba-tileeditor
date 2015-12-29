/* editorwindow.cpp
 * implementation fle for the main editor window */

#include <stdio.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

/* constructor which stores the app so we can quit later */
EditorWindow::EditorWindow(QApplication* app) {
    this->app = app;
}

/* set the map and palette areas */
void EditorWindow::setAreas( QGraphicsScene* map, QGraphicsScene* palette) {
    this->map_scene = map;
    this->palette_scene = palette;
}

/* set up all the signal and slot triggers for each action */
void EditorWindow::setup_triggers(Ui_MainWindow* ui) {
    /* the main window actions */
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
    QObject::connect(ui->actionChange_Properties, SIGNAL(triggered()), this, SLOT(on_change_properties()));
}

/* called when the various actions are taken */
void EditorWindow::on_new() {
    NewDialog* dialog = new NewDialog();
    Ui_NewMapDialog ui;
    ui.setupUi(dialog);

    /* add the options for tile maps - these are regular, not affine ones... */
    ui.comboBox->addItem("128x128 (Affine Only)");
    ui.comboBox->addItem("256x256");
    ui.comboBox->addItem("256x512 (Regular Only)");
    ui.comboBox->addItem("512x256 (Regular Only)");
    ui.comboBox->addItem("512x512");
    ui.comboBox->addItem("1024x1024 (Affine Only)");

    /* setup the triggers for this thing */
    dialog->setup_triggers(&ui); 
    dialog->exec();

    /* get the outcome of this */
    int choice = dialog->getSelection();

    /* if we actually chose an image size */
    if (choice != -1) {


    }
}

void EditorWindow::on_open() {
    QMessageBox msgBox;
    msgBox.setText("Open is not implemented yet :(");
    msgBox.exec(); 
}

void EditorWindow::on_save() {
    QMessageBox msgBox;
    msgBox.setText("Save is not implemented yet :(");
    msgBox.exec(); 
}

void EditorWindow::on_save_as() {
    QMessageBox msgBox;
    msgBox.setText("Save as is not implemented yet :(");
    msgBox.exec(); 
}

void EditorWindow::on_change_properties() {
    /* get the file name of the file they wish to use */
    QString file = QFileDialog::getOpenFileName(this,
         tr("Set Image"), "", tr("Image Files (*.png)"));

    /* check if the string is null */
    if (file.isNull()) {
        return;
    }

    /* set the image itself */
    tiles.load(file);

    /* display it in the bottom area */
    QPixmap p = QPixmap::fromImage(tiles);
    palette_scene->clear();
    palette_scene->addPixmap(p);
}

void EditorWindow::on_quit() {
    app->exit();
}

void EditorWindow::on_undo() {
    QMessageBox msgBox;
    msgBox.setText("Undo is not implemented yet :(");
    msgBox.exec(); 
}

void EditorWindow::on_redo() {
    QMessageBox msgBox;
    msgBox.setText("Redo is not implemented yet :(");
    msgBox.exec();
}

void EditorWindow::on_cut() {
    QMessageBox msgBox;
    msgBox.setText("Cut is not implemented yet :(");
    msgBox.exec(); 
}

void EditorWindow::on_copy() {
    QMessageBox msgBox;
    msgBox.setText("Copy is not implemented yet :(");
    msgBox.exec(); 
}

void EditorWindow::on_paste() {
    QMessageBox msgBox;
    msgBox.setText("Paste is not implemented yet :(");
    msgBox.exec();
}

void EditorWindow::on_grid() {
    QMessageBox msgBox;
    msgBox.setText("Grid is not implemented yet :(");
    msgBox.exec();
}

void EditorWindow::on_zoom_in() {
    QMessageBox msgBox;
    msgBox.setText("Zoom is not implemented yet :(");
    msgBox.exec();
}

void EditorWindow::on_zoom_out() {
    QMessageBox msgBox;
    msgBox.setText("Zoom is not implemented yet :(");
    msgBox.exec();
}

