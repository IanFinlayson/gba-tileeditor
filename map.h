/* map.h
 * the actual map model which stores the tile information */

#include <QImage>
#include <QPixmap>
#include <string>

class Map {
    private:
        int width, height;
        int* tiles;

    public:
        Map(int width, int height);
        Map();
        ~Map();
        QPixmap getPixmap(QImage* tiles);
        void write(const std::string& filename);
        bool read(const std::string& filename);
        int getWidth();
        int getHeight();
};



