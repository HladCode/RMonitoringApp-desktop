#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "AuthorizeWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

     QMenuBar* m;
     QMenu *mAuth;
     QAction* aAuth;
     AuthorizeWindow* wAuth;

     QDockWidget *leftDockWidget;
     QListView* sensors_view;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:

};
#endif // MAINWINDOW_H
