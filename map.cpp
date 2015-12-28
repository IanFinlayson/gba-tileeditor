/* map.cpp
 * the actual map model which stores the tile information */

#include "map.h"

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;
    this->tiles = new int[width * height];

    for (int i = 0; i < width * height; i++) {
        tiles[i] = 0;
    }
}



