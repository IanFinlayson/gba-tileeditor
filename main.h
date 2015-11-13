/* main.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef MAIN_H
#define MAIN_H

#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>

class EditorWindow : public QMainWindow {
    Q_OBJECT

    private:
        /* pointer to the main app so we can close it */
        QApplication* app;

        /* pointers to the map and palette area */
        QGraphicsScene* map;
        QGraphicsScene* palette;

        /* the image used as the tile sheet */
        QImage tiles;


    public:
        EditorWindow(QApplication* app);
        void setAreas(QGraphicsScene* map, QGraphicsScene* palette);
        void setup_triggers(Ui_MainWindow* ui);

    public slots:
        void on_new( );
        void on_open( );
        void on_save( );
        void on_save_as( );
        void on_set_image( );
        void on_quit( );

        void on_undo( );
        void on_redo( );
        void on_cut( );
        void on_copy( );
        void on_paste( );

        void on_grid( );
        void on_zoom_in( );
        void on_zoom_out( );
};

#endif

