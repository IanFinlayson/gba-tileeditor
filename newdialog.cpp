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
    printf("HI\n");
    QObject::connect(nd->actionOK, SIGNAL(triggered()), this, SLOT(nd_ok()));
    QObject::connect(nd->actionCancel, SIGNAL(triggered()), this, SLOT(nd_cancel()));
    QObject::connect(nd->actionSetImage, SIGNAL(triggered()), this, SLOT(nd_set_image()));
}

NewDialog::NewDialog() {

}

void NewDialog::nd_ok() {
    printf("AYYYYYYYYYYY\n");
    fflush(stdout);
    QMessageBox msgBox;
    msgBox.setText("OK");
    msgBox.exec();
}

void NewDialog::nd_cancel() {
    QMessageBox msgBox;
    msgBox.setText("Cancel");
    msgBox.exec();
}

void NewDialog::nd_set_image() {
    QMessageBox msgBox;
    msgBox.setText("setimage");
    msgBox.exec();
}

