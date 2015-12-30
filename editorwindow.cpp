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

/* shows a generic popup message */
void popup(const char* message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("GBA Tile Editor");
    msgBox.setText(message);
    msgBox.exec(); 
}

/* constructor which stores the app so we can quit later */
EditorWindow::EditorWindow(QApplication* app) {
    this->app = app;
    map = NULL;
    tiles_loaded = false;
    setWindowTitle("GBA Tile Editor");
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
    if (!tiles_loaded) {
        popup("Set a tile image file first!");
        return;
    }


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
        switch (choice) {
            case 0: map = new Map(128, 128); break;
            case 1: map = new Map(256, 256); break;
            case 2: map = new Map(256, 512); break;
            case 3: map = new Map(512, 256); break;
            case 4: map = new Map(512, 512); break;
            case 5: map = new Map(1024, 1024); break;
        }

        /* apply the map */
        QPixmap p = map->getPixmap(&tiles);
        map_scene->clear();
        map_scene->addPixmap(p);
    }
}

void EditorWindow::on_open() {
    if (!tiles_loaded) {
        popup("Set a tile image file first!");
        return;
    }

    filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Map Headers (*.h)"));
    if (filename != "") {
        filename_valid = true;
        if (map) {
            delete map;
        }
        map = new Map();
        if (map->read(filename.toStdString())) {
            /* apply the map */
            QPixmap p = map->getPixmap(&tiles);
            map_scene->clear();
            map_scene->addPixmap(p);
        } else {
            popup("The file does not appear to be a valid map header");
        }
    }
}

/* used for saving stuff */
QString EditorWindow::get_save_name() {
    QString f = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Map Headers (*.h)"));
    return f;
}

void EditorWindow::save_to_file() {
    if (map) {
        map->write(filename.toStdString());
    } else {
        popup("There is nothing to save yet!");
    }
}

void EditorWindow::on_save() {
    if (filename_valid) {
        save_to_file();
    } else {
        on_save_as();
    }
}

void EditorWindow::on_save_as() {
    filename_valid = true;
    filename = get_save_name();
    if (filename != "") {
        save_to_file();
    }
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
    tiles_loaded = true;

    /* display it in the bottom area */
    QPixmap p = QPixmap::fromImage(tiles);
    palette_scene->clear();
    palette_scene->addPixmap(p);
}

void EditorWindow::on_quit() {
    app->exit();
}

void EditorWindow::on_undo() {
    popup("Undo is not implemented yet :(");
}

void EditorWindow::on_redo() {
    popup("Redo is not implemented yet :(");
}

void EditorWindow::on_cut() {
    popup("Cut is not implemented yet :(");
}

void EditorWindow::on_copy() {
    popup("Copy is not implemented yet :(");
}

void EditorWindow::on_paste() {
    popup("Paste is not implemented yet :(");
}

void EditorWindow::on_grid() {
    popup("Grid is not implemented yet :(");
}

void EditorWindow::on_zoom_in() {
    popup("Zoom is not implemented yet :(");
}

void EditorWindow::on_zoom_out() {
    popup("Zoom is not implemented yet :(");
}

