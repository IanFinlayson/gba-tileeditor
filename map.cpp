/* map.cpp
 * the actual map model which stores the tile information */

#include <QFileInfo>
#include <stdio.h> 
#include "map.h"

Map::Map(int width, int height) {
    /* convert pixel sizes into tile sizes */
    this->width = width / 8;
    this->height = height / 8;
    this->tiles = new int[width * height];

    for (int i = 0; i < width * height; i++) {
        tiles[i] = rand() % 16;
    }
}

void Map::write(const std::string& filename) {
    FILE* f = fopen(filename.c_str(), "w");

    /* find the name which is filename with .h cut off */
    QFileInfo info(filename.c_str());
    std::string name = info.baseName().toStdString();

    /* write preamble stuff */
    fprintf(f, "#define %s_width %d\n", name.c_str(), width);
    fprintf(f, "#define %s_height %d\n\n", name.c_str(), height);
    fprintf(f, "const unsigned short %s [] = {\n    ", name.c_str());

    /* for each tile in the map */
    int counter = 0;
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            /* get the tile number we want */
            int tileno = tiles[row * width + col];

            /* dump it into the file */
            fprintf(f, "0x%04x, ", tileno);

            counter++;
            if (counter >= 9) {
                fprintf(f, "\n    ");
                counter = 0;
            }

        }
    }

    /* write postamble stuff */
    if (counter) {
        fprintf(f, "\n");
    }
    fprintf(f, "};\n\n");

    fclose(f);
}

QPixmap Map::getPixmap(QImage* tile_image) {
    /* create an image which we can draw into */
    QImage image(width * 8, height * 8, QImage::Format_RGB555);

    /* for each tile in the map */
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            /* get the tile number we want */
            int tileno = tiles[row * width + col];

            /* get the x, y position of this tile in the tile image
             * these represent the upper left hand corner of the tile */
            int tilex = (tileno * 8) % (tile_image->width());
            int tiley = (tileno * 8) / (tile_image->width());

            /* get the x, y position of this tile in the map image
             * ditto for upper left corner */
            int mapx = col * 8;
            int mapy = row * 8;

            /* now we will blit the whole tile manually */
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    QRgb color = tile_image->pixel(tilex + i, tiley + j);
                    image.setPixel(mapx + i, mapy + j, color); 
                }
            } 
        }
    }


    /* return a pixmap out of this image */
    return QPixmap::fromImage(image);
}



