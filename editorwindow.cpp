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
    current_tile = 0;
    just_saved = true;
}

/* set the map and palette areas */
void EditorWindow::set_areas( QGraphicsScene* map, QGraphicsScene* palette) {
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

/* called when the user creates a new map */
void EditorWindow::on_new() {
    /* if there are no tiles, we must bail */
    if (!tiles_loaded) {
        popup("Set a tile image file first!");
        return;
    }

    /* check if we should save first */
    if (!just_saved) {
        if (!check_save()) {
            return;
        }
    }

    /* create our new map dialog */
    NewDialog* dialog = new NewDialog();
    Ui_NewMapDialog ui;
    ui.setupUi(dialog);

    /* add the options for tile maps */
    ui.comboBox->addItem("16x16 (Affine Only)");
    ui.comboBox->addItem("32x32");
    ui.comboBox->addItem("32x64 (Regular Only)");
    ui.comboBox->addItem("64x32 (Regular Only)");
    ui.comboBox->addItem("64x64");
    ui.comboBox->addItem("128x128 (Affine Only)");

    /* setup the triggers for this thing and fire it */
    dialog->setup_triggers(&ui); 
    dialog->exec();

    /* get the outcome of this */
    int choice = dialog->get_selection();

    /* if we actually chose an image size */
    if (choice != -1) {
        /* create the appropriately sized map */
        switch (choice) {
            case 0: map = new Map(16, 16); break;
            case 1: map = new Map(32, 32); break;
            case 2: map = new Map(32, 64); break;
            case 3: map = new Map(64, 32); break;
            case 4: map = new Map(64, 64); break;
            case 5: map = new Map(128, 128); break;
        }

        /* apply the map */
        QPixmap p = map->get_pixmap(&tiles);
        map_scene->clear();
        map_scene->addPixmap(p);
        just_saved = true;
    }
}

/* called when the user quits via the window manager */
void EditorWindow::closeEvent(QCloseEvent* event) {
    if (!just_saved) {
        if (!check_save()) {
            event->ignore();
            return;
        }
    }
    event->accept();
}

/* ask the user if they want to save first - returns true if we should
 * carry on and false if we should abort whatever it is we are doing */
bool EditorWindow::check_save() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("GBA Tile Editor");
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes first?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    switch (msgBox.exec()) {
        case QMessageBox::Save:
            on_save();
            return true;
        case QMessageBox::Discard:
            return true;
        case QMessageBox::Cancel:
            return false;
        default:
            return false;
    }
}

/* called when the user chooses the open map option */
void EditorWindow::on_open() {
    /* if we have no tiles, bail */
    if (!tiles_loaded) {
        popup("Set a tile image file first!");
        return;
    }

    /* check if we need to save first */
    if (!just_saved) {
        if (!check_save()) {
            return;
        }
    }

    /* get the file name */
    filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Map Headers (*.h)"));
    if (filename != "") {
        filename_valid = true;
        if (map) {
            delete map;
        }
        map = new Map();
        if (map->read(filename.toStdString())) {
            /* apply the map */
            QPixmap p = map->get_pixmap(&tiles);
            map_scene->clear();
            map_scene->addPixmap(p);
            just_saved = true;
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

/* actually perform the save */
void EditorWindow::save_to_file() {
    if (map) {
        map->write(filename.toStdString());
        just_saved = true;
    } else {
        popup("There is nothing to save yet!");
    }
}

/* called when the user chooses the save option */
void EditorWindow::on_save() {
    if (filename_valid) {
        save_to_file();
    } else {
        on_save_as();
    }
}

/* called when user chooses save as option */
void EditorWindow::on_save_as() {
    filename_valid = true;
    filename = get_save_name();
    if (filename != "") {
        save_to_file();
    }
}

/* called when the user chooses a tile image */
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

/* called when the palette is clicked */
void EditorWindow::palette_click(int x, int y) {
    /* if there is no palette, then bail */
    if (!tiles_loaded) {
        return;
    }

    /* if the click is out of bounds, then bail */
    if (x > tiles.width() || y > tiles.height()) {
        return;
    }

    /* figure out which tile they clicked */
    int tile = (y / 8) * (tiles.width() / 8) + (x / 8);

    /* set the current tile based on this */
    current_tile = tile;
}

/* called when the map is clicked */
void EditorWindow::map_click(int x, int y) {
    /* if there is no map, then bail */
    if (!map) {
        return;
    }

    /* if the click is out of bounds, then bail */
    if (x > (map->get_width() * 8) || y > (map->get_height() * 8)) {
        return;
    }

    /* figure out which tile they clicked */
    int tile = (y / 8) * map->get_width() + (x / 8);

    /* apply this tile */
    just_saved = false;
    map->set_tile(tile, current_tile);
    QPixmap p = map->get_pixmap(&tiles);
    map_scene->clear();
    map_scene->addPixmap(p);
}

/* called when the user quits from the application */
void EditorWindow::on_quit() {
    if (!just_saved) {
        if (!check_save()) {
            return;
        }
    }
    app->exit();
}

/* called when the user hits undo */
void EditorWindow::on_undo() {
    if (map) {
        map->undo();

        /* apply the map */
        QPixmap p = map->get_pixmap(&tiles);
        map_scene->clear();
        map_scene->addPixmap(p);
    }
}

/* called when the user hits redo */
void EditorWindow::on_redo() {
    if (map) {
        map->redo();

        /* apply the map */
        QPixmap p = map->get_pixmap(&tiles);
        map_scene->clear();
        map_scene->addPixmap(p);
    }
}




/* TODO work on these */
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

