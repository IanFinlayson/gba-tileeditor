/* map.h
 * the actual map model which stores the tile information */

#ifndef MAP_H
#define MAP_H

#include <QImage>
#include <QPixmap>
#include <string>
#include <stack>

class Map {
    private:
        /* width and height measured in tiles */
        int width, height;
        int* tiles;

        /* whether this is a regular map (if not it's affine) */
        bool regular;

        /* undo and redo stacks store the tile information */
        std::stack<int*> undo_stack;
        std::stack<int*> redo_stack;

        /* do the tile lookup in the order that the GBA requires */
        int* lookup_tile(int& sb, int& row, int& col, int& above, int& left);

    public:
        Map(int width, int height, bool regular);
        Map();
        ~Map();

        /* get a pixmap of what this map will look like when rendered */
        QPixmap get_pixmap(QImage* tiles, bool grid_mode, QColor grid_color);

        /* load/save the map from/to a file */
        void write(const std::string& filename);
        bool read(const std::string& filename);

        void start_drag();

        /* set a tile in the map to a new value */
        void set_tile(int index, int tile_no);
        int get_tile(int index);

        /* get the dimensions */
        int get_width();
        int get_height();

        /* undo or redo last operation */
        void undo();
        void redo();
};

#endif


