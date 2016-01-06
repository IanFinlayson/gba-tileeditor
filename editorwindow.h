/* main.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include "ui_mainwindow.h"
#include "map.h"

#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>

class EditorWindow : public QMainWindow {
    Q_OBJECT

    private:
        /* pointer to the main app so we can close it */
        QApplication* app;

        /* pointers to the map and palette area */
        QGraphicsScene* map_scene;
        QGraphicsScene* palette_scene;

        /* the image used as the tile sheet */
        bool tiles_loaded;
        QImage tiles;
        QImage tiles_grid;
        QColor grid_color;

        /* the currently selected tile used for drawing */
        int current_tile;

        /* pointer to the actual map data */
        Map* map; 

        /* the filename, and whether it is valid at all */
        bool filename_valid;
        QString  filename;

        /* used for saving stuff */
        QString get_save_name();
        void save_to_file();

        /* whether we have just saved our work */
        bool just_saved;
        bool check_save();

        /* the zoom factor we are using */
        int zoom_factor;

        /* whether we are draing the grid over the palette and map */
        bool grid_mode;

        /* refreshes the map or palette views */
        void refresh_map();
        void refresh_palette();

    public:
        EditorWindow(QApplication* app);
        void set_areas(QGraphicsScene* map, QGraphicsScene* palette);
        void setup_triggers(Ui_MainWindow* ui);
        void palette_click(int x, int y);
        void map_click(int x, int y); 
        void closeEvent(QCloseEvent* event);

        public slots:
        /* actions for the main window */
        void on_new();
        void on_open();
        void on_save();
        void on_save_as();
        void on_change_properties();
        void on_quit();
        void on_undo();
        void on_redo();
        void on_zoom_in();
        void on_zoom_out();
        void on_grid();
};

#endif

