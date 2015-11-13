/* mapview.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef MAPVIEW_h
#define MAPVIEW_h

#include <QGraphicsView>

class MapView : public QGraphicsView {
    Q_OBJECT

    public:
        MapView(QWidget* parent);
        void mousePressEvent(QMouseEvent* event);
};

#endif

