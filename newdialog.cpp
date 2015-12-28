/* newdialog.cpp
 * implementation of new map dialog class */

#include <stdio.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "editorwindow.h"
#include "newdialog.h"
#include "ui_mainwindow.h"
#include "ui_newmap.h"

void NewDialog::setup_triggers(Ui_NewMapDialog* nd) {
    /* the new map dialog */
    QObject::connect(nd->actionOK, SIGNAL(triggered()), this, SLOT(nd_ok()));
    QObject::connect(nd->actionCancel, SIGNAL(triggered()), this, SLOT(nd_cancel()));
    combo = nd->comboBox;
}

NewDialog::NewDialog() {
    success = false;
    option = 0;
}

void NewDialog::nd_ok() {
    success = true;
    option = combo->currentIndex();
    close();
}

int NewDialog::getSelection() {
    if (success) {
        return option;
    } else {
        return -1;
    }
}

void NewDialog::nd_cancel() {
    success = false;
    close();
}

