/* paletteview.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef PALETTEVIEW_H
#define PALETTEVIEW_H

#include <QGraphicsView>

class EditorWindow;

class PaletteView : public QGraphicsView {
    Q_OBJECT

    private:
        EditorWindow* window;

    public:
        PaletteView(QWidget* parent);
        void set_window(EditorWindow* window);
        void mousePressEvent (QMouseEvent* event);
};

#endif

