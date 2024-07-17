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
Map::Map(int width, int height, bool regular) {
    /* convert pixel sizes into tile sizes */
    this->width = width;
    this->height = height;
    this->regular = regular;
    this->tiles = new int[width * height];

    for (int i = 0; i < width * height; i++) {
        tiles[i] = 0;
    }
}

/* access the width of the map (in tiles) */
int Map::get_width() {
    return width;
}

/* access the width of the map (in tiles) */
int Map::get_height() {
    return height;
}

/* make a blank map */
Map::Map() {
    width = 0;
    height = 0;
    regular = true;
    tiles = NULL;
}

/* clear the memory for this map */
Map::~Map() {
    if (tiles) {
        delete [] tiles;
    }

    while (!undo_stack.empty()) {
        int* tmp = undo_stack.top();
        undo_stack.pop();
        delete [] tmp;
    }

    while (!redo_stack.empty()) {
        int* tmp = redo_stack.top();
        redo_stack.pop();
        delete [] tmp;
    }
}

/* gets a tile from the map using the complex indexing required 
 * must be called until it returns NULL */
int* Map::lookup_tile(int& sb, int& row, int& col, int& above, int& left) {
    /* if it's an affine background, then we must do them strictly in order */
    if (!regular) {
        /* we just go tile by tile */
        if (row == height) {
            return NULL;
        }

        /* grab the value */
        int* value = &tiles[row * width + col];

        /* update counters */
        col++;
        if (col == width) {
            col = 0;
            row++;
        }

        return value;
    }

    /* count the screen block we have to write */
    int num_sbs = (width * height) / 1024;

    /* check if we are done */
    if (sb == num_sbs) {
        return NULL;
    }
    
    /* get the next one based off of the current indices */
    int realrow = row + 32 * above;
    int realcol = col + 32 * left;

    /* update the column */
    col++;

    /* if the column is out, move to the next row */
    if (col == 32) {
        row++;
        col = 0;
    }

    /* if the row is out, move to next screen block */
    if (row == 32) { 
        /* if we just did the last screen block in a row of screen blocks */
        int last;
        switch (num_sbs) {
            case 1:
                last = 1;
                break;
            case 2:
                if (width == 32)
                    last = 1;
                else
                    last = 0;
                break;
            case 4:
                if (sb == 1 || sb == 3)
                    last = 1;
                else
                    last = 0;
                break;
            case 16:
                if ((sb + 1) % 4 == 0)
                    last = 1;
                else
                    last = 0;
                break;
        }

        /* if it WAS the last in a row, none are left and one more is above
         * otherwise, one more is to the left */
        if (last) {
            left = 0;
            above++;
        } else {
            left++;
        }

        /* now update to the next block */
        sb++;
        row = 0;
    }

    /* finally return the shit */
    return &tiles[realrow * width + realcol];
}

/* read this map in from a file */
bool Map::read(const std::string& filename) {
    std::string line;
    std::ifstream f(filename.c_str());

    /* check our little signature is there */
    if (!std::getline(f, line)) return false;
    if (line != "/* created by GBA Tile Editor") {
        return false;
    }

    /* read our map tpye (regular or affine) */
    if (!std::getline(f, line)) return false;
    else {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "regular") {
            this->regular = true;
        } else if (type == "affine") {
            this->regular = false;
        } else {
            return false;
        } 
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

    /* start the complex indexing loop operation */
    int sb = 0;
    int above = 0;
    int left = 0;
    int row = 0;
    int col = 0;
    int* tile;

    /* grab the next tile we need to write into */
    while ((tile = lookup_tile(sb, row, col, above, left))) {
        /* read the value */
        if (! (f >> value)) {
            return false;
        }

        /* convert to a number */
        int val = strtol(value.c_str(), NULL, 16); 

        /* now we need to put it into its place */
        *tile = val;
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
    fprintf(f, "/* created by GBA Tile Editor\n");
    fprintf(f, "   %s map */\n\n", regular ? "regular" : "affine");
    fprintf(f, "#define %s_width %d\n", name.c_str(), width);
    fprintf(f, "#define %s_height %d\n\n", name.c_str(), height);

    /* affine backgrounds are 8-bit values, regular are 16-bit */
    if (!regular) {
        fprintf(f, "const unsigned char %s [] = {\n    ", name.c_str());
    } else {
        fprintf(f, "const unsigned short %s [] = {\n    ", name.c_str());
    }

    /* start line break counter */
    int counter = 0;

    /* start the complex indexing loop operation */
    int sb = 0;
    int above = 0;
    int left = 0;
    int row = 0;
    int col = 0;
    int* tile;

    /* grab the next tile we need to write into */
    while ((tile = lookup_tile(sb, row, col, above, left))) {
        /* dump it into the file
         * regular backgrounds use 16-bit indices, affine ones use 8-bit ones */
        if (!regular) {
            fprintf(f, "0x%02x, ", (unsigned char)(*tile));
        } else {
            fprintf(f, "0x%04x, ", (unsigned short)(*tile));
        }

        counter++;
        if (counter >= 9) {
            fprintf(f, "\n    ");
            counter = 0;
        }
    }

    /* write postamble stuff */
    if (counter) {
        fprintf(f, "\n");
    }
    fprintf(f, "};\n\n");

    fclose(f);
}

/* pushes map state to undo stack when drag is started */
void Map::start_drag() {
    int* temp = new int[width * height];
    for (int i = 0; i < width * height; i++) {
        temp[i] = tiles[i];
    }

    undo_stack.push(temp);
}

/* modify the tile */
void Map::set_tile(int index, int tile_no) {
    tiles[index] = tile_no; 
}

int Map::get_tile(int index) {
    return tiles[index];
}

/* get a pixmap from this Map which can be shown in a QT view */
QPixmap Map::get_pixmap(QImage* tile_image, bool grid_mode, QColor grid_color) {
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
            int tiley = ((tileno * 8) / (tile_image->width())) * 8;

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

    /* draw the grid, if needed */
    if (grid_mode) {

        /* for each row 7 of a tile */
        for (int i = 7; i < image.height(); i += 8) {
            /* for each column */
            for (int j = 0; j < image.width(); j++) {
                image.setPixel(j, i, grid_color.rgba());
            }
        }

        /* for each column 7 of a tile */
        for (int i = 7; i < image.width(); i += 8) {
            /* for each column */
            for (int j = 0; j < image.height(); j++) {
                image.setPixel(i, j, grid_color.rgba());
            }
        }
    }

    /* return a pixmap out of this image */
    return QPixmap::fromImage(image);
}

/* undo the last move made */
void Map::undo() {
    if (undo_stack.empty()) {
        return;
    }

    /* pop off the last one */
    int* restore = undo_stack.top();
    undo_stack.pop();

    /* replace tiles with it */
    int* current = tiles;
    tiles = restore;

    /* push this onto redo stack */
    redo_stack.push(current);
}

/* redo the last move made */
void Map::redo() {
    if (redo_stack.empty()) {
        return;
    }

    /* pop off the last one */
    int* restore = redo_stack.top();
    redo_stack.pop();

    /* replace tiles with it */
    int* current = tiles;
    tiles = restore;

    /* push this onto redo stack */
    undo_stack.push(current);
}


