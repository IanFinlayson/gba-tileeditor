/* mapview.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef MAPVIEW_h
#define MAPVIEW_h

#include <QGraphicsView>

class EditorWindow;

class MapView : public QGraphicsView {
    Q_OBJECT

    private:
        EditorWindow* window;

    public:
        MapView(QWidget* parent);
        void set_window(EditorWindow* window);
        void mousePressEvent(QMouseEvent* event);
};

#endif

