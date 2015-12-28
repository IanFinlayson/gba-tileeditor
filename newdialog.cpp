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
}

NewDialog::NewDialog() {

}

void NewDialog::nd_ok() {
    QMessageBox msgBox;
    msgBox.setText("OK");
    msgBox.exec();
}

void NewDialog::nd_cancel() {
    QMessageBox msgBox;
    msgBox.setText("Cancel");
    msgBox.exec();
}

