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

void PaletteView::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) {
        return;
    }

    char message[64];
    sprintf(message, "(%d, %d)", e->x(), e->y());

    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}


