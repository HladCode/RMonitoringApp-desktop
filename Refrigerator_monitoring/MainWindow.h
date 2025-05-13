#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

     QMenuBar* m;
     QMenu *mAuth;

     QDockWidget *leftDockWidget;
     QListView* sensors_view;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
