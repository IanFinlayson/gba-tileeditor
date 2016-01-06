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
    grid_mode = false;
    map = NULL;
    tiles_loaded = false;
    grid_color = QColor(255, 0, 0);
    setWindowTitle("GBA Tile Editor");
    current_tile = 0;
    just_saved = true;
    zoom_factor = 2;
}

/* set the map and palette areas */
void EditorWindow::set_areas(QGraphicsScene* map, QGraphicsScene* palette) {
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
    QObject::connect(ui->actionZoom_In, SIGNAL(triggered()), this, SLOT(on_zoom_in()));
    QObject::connect(ui->actionZoom_Out, SIGNAL(triggered()), this, SLOT(on_zoom_out()));
    QObject::connect(ui->actionChange_Properties, SIGNAL(triggered()), this, SLOT(on_change_properties()));
    QObject::connect(ui->actionShow_Grid, SIGNAL(triggered()), this, SLOT(on_grid()));
}

/* refresh the map area */
void EditorWindow::refresh_map() {
    if (!tiles_loaded || !map) {
        return;
    }

    QPixmap p = map->get_pixmap(&tiles, grid_mode, grid_color);

    /* zoom in, if needed */
    if (zoom_factor > 1) {
        p = p.scaled(p.width() * zoom_factor, p.height() * zoom_factor, 
                Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }

    map_scene->clear();
    map_scene->addPixmap(p);
}

/* refresh the palette area */
void EditorWindow::refresh_palette() {
    /* get a pixmap, either with or without grid lines */
    QPixmap p; 
    if (grid_mode) {
        p = QPixmap::fromImage(tiles_grid);
    } else {
        p = QPixmap::fromImage(tiles);
    }

    /* zoom in, if needed */
    if (zoom_factor > 1) {
        p = p.scaled(p.width() * zoom_factor, p.height() * zoom_factor, 
                Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }
    palette_scene->clear();
    palette_scene->addPixmap(p);
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
        refresh_map();
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

/* called when the user toggles the grid option (default is off) */
void EditorWindow::on_grid() {
    grid_mode = !grid_mode;
    refresh_map();
    refresh_palette();
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
            refresh_map();
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

    /* compute the tile image but with grid lines */
    tiles_grid = tiles.copy();

    /* for each row 7 of a tile */
    for (int i = 7; i < tiles_grid.height(); i += 8) {
        /* for each column */
        for (int j = 0; j < tiles_grid.width(); j++) {
            tiles_grid.setPixel(j, i, grid_color.rgba());
        }
    }

    /* for each column 7 of a tile */
    for (int i = 7; i < tiles_grid.width(); i += 8) {
        /* for each column */
        for (int j = 0; j < tiles_grid.height(); j++) {
            tiles_grid.setPixel(i, j, grid_color.rgba());
        }
    }

    /* display it in the bottom area */
    refresh_palette();
}

/* called when the palette is clicked */
void EditorWindow::palette_click(int x, int y) {
    /* if there is no palette, then bail */
    if (!tiles_loaded) {
        return;
    }

    /* scale the click */
    x /= zoom_factor;
    y /= zoom_factor;

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

    /* scale the click */
    x /= zoom_factor;
    y /= zoom_factor;

    /* if the click is out of bounds, then bail */
    if (x > (map->get_width() * 8) || y > (map->get_height() * 8)) {
        return;
    }

    /* figure out which tile they clicked */
    int tile = (y / 8) * map->get_width() + (x / 8);

    /* apply this tile */
    just_saved = false;
    map->set_tile(tile, current_tile);
    refresh_map();
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
        refresh_map();
    }
}

/* called when the user hits redo */
void EditorWindow::on_redo() {
    if (map) {
        map->redo();

        /* apply the map */
        refresh_map();
    }
}

/* called when the user zooms in */
void EditorWindow::on_zoom_in() {
    if (zoom_factor < 8) {
        zoom_factor *= 2;
    }
    if (map) {
        refresh_map();
    }
    if (tiles_loaded) {
        refresh_palette();
    }
}

/* called when the user zooms out */
void EditorWindow::on_zoom_out() {
    if (zoom_factor > 1) {
        zoom_factor /= 2;
    }
    if (map) {
        refresh_map();
    }
    if (tiles_loaded) {
        refresh_palette();
    }
}



