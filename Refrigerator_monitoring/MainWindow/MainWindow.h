#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "DialogWindows/AuthorizeWindow/AuthorizeWindow.h"
#include "DialogWindows/RegistrationWindow/registrationWindow.h"
#include "DialogWindows/SettingsWindow/settingsWindow.h"
#include "MainWindow/unitListModel/unitListModel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

     QMenuBar* m;
     QMenu *mAuth;
     QMenu* mUnits;
     QMenu* mUsers;
     QAction* aAuth;
     AuthorizeWindow* wAuth;
     RegistrationWindow* wReg;
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
     QDateEdit* dateData;
     QTimeEdit* startTimeEdit;
     QTimeEdit* endTimeEdit;
     QPushButton* bGetSensorData;

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
    void slotGetSensorData();
    void slotStartTimeChanged();
    void slotEndTimeChanged();
};
#endif // MAINWINDOW_H
