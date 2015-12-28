/* map.h
 * the actual map model which stores the tile information */

class Map {
    private:
        int width, height;
        int* tiles;

    public:
        Map(int width, int height);
};



