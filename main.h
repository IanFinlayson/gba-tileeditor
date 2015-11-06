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
        void on_new( );
        void on_open( );
        void on_save( );
        void on_save_as( );
        void on_set_image( );
        void on_quit( );

        void on_undo( );
        void on_redo( );
        void on_cut( );
        void on_copy( );
        void on_paste( );

        void on_grid( );
        void on_zoom_in( );
        void on_zoom_out( );
};

