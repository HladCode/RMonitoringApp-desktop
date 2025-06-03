#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "DialogWindows/AuthorizeWindow/AuthorizeWindow.h"
#include "DialogWindows/SettingsWindow/settingsWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

     QMenuBar* m;
     QMenu *mAuth;
     QMenu* mUnits;
     QAction* aAuth;
     AuthorizeWindow* wAuth;
     settingsWindow *wSettings;

     QDockWidget *leftDockWidget;
     QComboBox *placesBox;
     QComboBox *unitsBox;
     QWidget* bufleftDockWidget;
     QVBoxLayout* leftDockVBL;
     QListView* sensors_view;

     QSettings settings;

     QString jwt_token;
     QString jwt_refresh_token;
     QString url;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void readSettings();    

protected slots:
    void slotSaveDataFromAuth();
    void slotMenuTriggered(QAction* a);
    void slotWSeetingsChanged();
};
#endif // MAINWINDOW_H
