#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "DialogWindows/AuthorizeWindow/AuthorizeWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

     QMenuBar* m;
     QMenu *mAuth;
     QAction* aAuth;
     AuthorizeWindow* wAuth;

     QDockWidget *leftDockWidget;
     QListView* sensors_view;

     QSettings settings;

     QString jwt_token;
     QString jwt_refresh_token;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void readSettings();    

protected slots:
    void slotSaveDataFromAuth();
};
#endif // MAINWINDOW_H
