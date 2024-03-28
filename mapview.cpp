/* mapview.cpp
 * the map view implementation file */

#include <stdio.h>
#include <QMouseEvent>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "qnamespace.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

MapView::MapView(QWidget* parent) : QGraphicsView(parent), dragging(false) {

}

/* tell the component about the window its in */
void MapView::set_window(EditorWindow* window) {
    this->window = window;
}

/* mouse handlers */
void MapView::mouseMoveEvent(QMouseEvent* e) {
    if(!dragging) {
        return;
    }
    updateMapAt(e);
}

void MapView::updateMapAt(QMouseEvent* e) {
    /* find the position of our scroll bar */
    int scroll_x = horizontalScrollBar()->value();
    int scroll_y = verticalScrollBar()->value();

    /* apply the click onto the window - adjusted for scroll */
    window->map_click(e->x() + scroll_x, e->y() + scroll_y);
}


void MapView::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        dragging = false;
    }
}

void MapView::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        dragging = true;
        window->start_drag();

        updateMapAt(e);
    }
}
