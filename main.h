/* main.h
 * contains the class declaration for the EditorWindow
 * this has to be in a header file for QT to function */

#include "ui_mainwindow.h"

class EditorWindow : public QMainWindow {
    Q_OBJECT

    private:
        QApplication* app;

    public:
        EditorWindow(QApplication* app);
        void setup_triggers(Ui_MainWindow* ui);

    public slots:
        void close( );
};

