/* map.cpp
 * the actual map model which stores the tile information */

#include <QFileInfo>
#include <stdio.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "map.h"

/* construct a new map of a given size, and set to all zeroes */
Map::Map(int width, int height) {
    /* convert pixel sizes into tile sizes */
    this->width = width / 8;
    this->height = height / 8;
    this->tiles = new int[width * height];

    for (int i = 0; i < width * height; i++) {
        tiles[i] = 0;
    }
}

/* make a blank map */
Map::Map() {
    width = 0;
    height = 0;
    tiles = NULL;
}

/* clear the memory for this map */
Map::~Map() {
    if (tiles) {
        delete [] tiles;
    }
}

/* read this map in from a file */
bool Map::read(const std::string& filename) {
    std::string line;
    std::ifstream f(filename.c_str());

    /* check our little signature is there */
    if (!std::getline(f, line)) return false;
    if (line != "/* created by GBA Tile Editor */") {
        return false;
    }

    /* skip the blank line */
    if (!std::getline(f, line)) return false;

    /* read the width */
    if (!std::getline(f, line)) return false;
    else {
        std::istringstream iss(line);
        std::string def, name;
        int val;
        iss >> def >> name >> val;
        width = val;
    }

    /* read the height */
    if (!std::getline(f, line)) return false;
    else {
        std::istringstream iss(line);
        std::string def, name;
        int val;
        iss >> def >> name >> val;
        height = val;
    }

    /* skip the blank line and the declaration line */
    if (!std::getline(f, line)) return false;
    if (!std::getline(f, line)) return false;

    /* allocate space for the tiles */
    this->tiles = new int[width * height];

    /* read the pixel data */
    std::string value;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            /* read the value */
            if (! (f >> value)) {
                return false;
            }

            /* convert to a number */
            int val = strtol(value.c_str(), NULL, 16); 
            tiles[i*width + j] = val;
        }
    }

    return true;
}

/* write this map into a file */
void Map::write(const std::string& filename) {
    FILE* f = fopen(filename.c_str(), "w");

    /* find the name which is filename with .h cut off */
    QFileInfo info(filename.c_str());
    std::string name = info.baseName().toStdString();

    /* write preamble stuff */
    fprintf(f, "/* created by GBA Tile Editor */\n\n"); 
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

/* get a pixmap from this Map which can be shown in a QT view */
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



