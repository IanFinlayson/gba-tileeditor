/* map.h
 * the actual map model which stores the tile information */

#include <QImage>
#include <QPixmap>

class Map {
    private:
        int width, height;
        int* tiles;

    public:
        Map(int width, int height);
        QPixmap getPixmap(QImage* tiles);
};



