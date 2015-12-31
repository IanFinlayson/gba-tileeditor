/* paletteview.cpp
 * implementation of the palette view */

#include <stdio.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

/* constructors for the view objects */
PaletteView::PaletteView(QWidget* parent) : QGraphicsView(parent) {

}

/* tell the component about the window it's in */
void PaletteView::set_window(EditorWindow* window) {
    this->window = window; 
}

/* called when the mouse is pressed in this area */
void PaletteView::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) {
        return;
    }

    /* find the position of our scroll bar */
    int scroll_x = horizontalScrollBar()->value();
    int scroll_y = verticalScrollBar()->value();

    /* apply this palette click - taking scrolling into account */
    window->palette_click(e->x() + scroll_x, e->y() + scroll_y);
}


