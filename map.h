/* map.h
 * the actual map model which stores the tile information */

#include <QImage>
#include <QPixmap>
#include <string>
#include <stack>

class Map {
    private:
        int width, height;
        int* tiles;
        std::stack<int*> undo_stack;
        std::stack<int*> redo_stack;

    public:
        Map(int width, int height);
        Map();
        ~Map();
        QPixmap get_pixmap(QImage* tiles);
        void write(const std::string& filename);
        bool read(const std::string& filename);
        void set_tile(int index, int tile_no);
        int get_width();
        int get_height();
        void undo();
        void redo();
};



