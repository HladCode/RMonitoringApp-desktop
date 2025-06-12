#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtCharts>
#include "DialogWindows/AuthorizeWindow/AuthorizeWindow.h"
#include "DialogWindows/RegistrationWindow/registrationWindow.h"
#include "DialogWindows/SettingsWindow/settingsWindow.h"
#include "MainWindow/unitListModel/unitListModel.h"
#include "lib/ChartView/chartView.h"

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
     QDateTimeEdit* startDateTimeEdit;
     QDateTimeEdit* endDateTimeEdit;
     QPushButton* bGetSensorData;

     QCheckBox* isRealTime;
     //QPushButton* bStartRealTime;
     QTimer* realTimeTimer;
     QDateTime realTimeStartTime;


     QLineSeries *series;
     QChart *chart;
     QDateTimeAxis *axisX;
     QValueAxis *axisY;
     ChartView *chartView;
     QLabel* chartInfo;

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
    void slotPlusAxisX();
    void slotMinusAxisX();

    void slotIsRealTimeChecked(Qt::CheckState state);
    void slotUpdateRealTimeData();
};
#endif // MAINWINDOW_H
