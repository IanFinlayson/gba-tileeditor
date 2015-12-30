/* mapview.cpp
 * the map view implementation file */

#include <stdio.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

MapView::MapView(QWidget* parent) : QGraphicsView(parent) {

}

void MapView::set_window(EditorWindow* window) {
    this->window = window;
}

/* mouse handlers */
void MapView::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) {
        return;
    }

    window->map_click(e->x(), e->y());
}


