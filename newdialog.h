/* newdialgo.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef ND_H
#define ND_H

#include "ui_newmap.h"

#include <QGraphicsScene>
#include <QGraphicsView>

class NewDialog : public QDialog {
    Q_OBJECT

    private:


    public:
        NewDialog();
        void setup_triggers(Ui_NewMapDialog* nd);

    public slots:
        void nd_ok( );
        void nd_cancel( );
};

#endif

