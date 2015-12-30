/* paletteview.cpp
 * implementation of the palette view */

#include <stdio.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

/* constructors for the view objects */
PaletteView::PaletteView(QWidget* parent) : QGraphicsView(parent) {

}

void PaletteView::set_window(EditorWindow* window) {
    this->window = window; 
}

void PaletteView::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) {
        return;
    }

    window->palette_click(e->x(), e->y());
}


