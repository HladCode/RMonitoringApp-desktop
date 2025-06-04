#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "DialogWindows/AuthorizeWindow/AuthorizeWindow.h"
#include "DialogWindows/SettingsWindow/settingsWindow.h"
#include "MainWindow/unitListModel/unitListModel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

     QMenuBar* m;
     QMenu *mAuth;
     QMenu* mUnits;
     QAction* aAuth;
     AuthorizeWindow* wAuth;
     settingsWindow *wSettings;

     QMap<QString, QStringList> units;
     QDockWidget *leftDockWidget;
     QComboBox *placesBox;
     QComboBox *unitsBox;
     QPushButton *bApplyFilter;
     unitModel* unitList;
     QWidget* bufleftDockWidget;
     QVBoxLayout* leftDockVBL;
     QListView* sensors_view;

     QSettings settings;

     QNetworkAccessManager *manager;
     QString jwt_token;
     QString jwt_refresh_token;
     QString url;

     QErrorMessage* errMsg;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void readSettings();    
    void getUnits();

protected slots:
    void slotSaveDataFromAuth();
    void slotMenuTriggered(QAction* a);
    void slotWSeetingsChanged();
    void slotPlacesBox(const QString& place);
    void slotApplyFilterClicked();
};
#endif // MAINWINDOW_H
