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

    /* add the options for tile maps the default is regular */
    on_regular();

    /* set up the callbacks for changing the type of tile map it is */
    QObject::connect(nd->actionRegular, SIGNAL(triggered()), this, SLOT(on_regular()));
    QObject::connect(nd->actionAffine, SIGNAL(triggered()), this, SLOT(on_affine()));
}

/* set initial values */
NewDialog::NewDialog() {
    success = false;
    option = 0;
}

void NewDialog::on_regular() {
    combo->clear();
    combo->addItem("32x32");
    combo->addItem("32x64");
    combo->addItem("64x32");
    combo->addItem("64x64");
    regular = true;
}

void NewDialog::on_affine() {
    combo->clear();
    combo->addItem("16x16");
    combo->addItem("32x32");
    combo->addItem("64x64");
    combo->addItem("128x128");
    regular = false;
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

/* return whether the user chose a regular or affine background */
bool NewDialog::is_regular() {
    return regular;
}

/* called when cancel is pressed */
void NewDialog::nd_cancel() {
    success = false;
    close();
}

