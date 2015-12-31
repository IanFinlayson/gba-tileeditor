/* mapview.cpp
 * the map view implementation file */

#include <stdio.h>
#include <QMouseEvent>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

MapView::MapView(QWidget* parent) : QGraphicsView(parent) {

}

/* tell the component about the window its in */
void MapView::set_window(EditorWindow* window) {
    this->window = window;
}

/* mouse handlers */
void MapView::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) {
        return;
    }

    /* find the position of our scroll bar */
    int scroll_x = horizontalScrollBar()->value();
    int scroll_y = verticalScrollBar()->value();

    /* apply the click onto the window - adjusted for scroll */
    window->map_click(e->x() + scroll_x, e->y() + scroll_y);
}

