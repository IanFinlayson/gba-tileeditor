/* paletteview.h
 * contains the class declarations
 * this has to be in a header file for QT to function */

#ifndef PALETTEVIEW_H
#define PALETTEVIEW_H

#include <QGraphicsView>

class PaletteView : public QGraphicsView {
    Q_OBJECT

    public:
        PaletteView(QWidget* parent);
        void mousePressEvent (QMouseEvent* event);
};

#endif

