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

/* setup the OK and Cancel button actions for the dialog */
void NewDialog::setup_triggers(Ui_NewMapDialog* nd) {
    /* the new map dialog */
    QObject::connect(nd->actionOK, SIGNAL(triggered()), this, SLOT(nd_ok()));
    QObject::connect(nd->actionCancel, SIGNAL(triggered()), this, SLOT(nd_cancel()));
    combo = nd->comboBox;
}

/* set initial values */
NewDialog::NewDialog() {
    success = false;
    option = 0;
}

/* called when OK is pressed */
void NewDialog::nd_ok() {
    success = true;
    option = combo->currentIndex();
    close();
}

/* check which option the user chose in the dialog (-1 on cancel) */
int NewDialog::get_selection() {
    if (success) {
        return option;
    } else {
        return -1;
    }
}

/* called when cancel is pressed */
void NewDialog::nd_cancel() {
    success = false;
    close();
}

