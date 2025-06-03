#include "MainWindow/MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), settings("HladCode", "RMonitorig") {
    m = new QMenuBar;

    aAuth = new QAction("Login...");
    wSettings = new settingsWindow;
    wSettings->setUrl("http://78.137.37.34:1488"); // TODO: make default url be from QSettings by setURL
    url = wSettings->getUrl();
    wAuth = new AuthorizeWindow(url);
    m->addAction(aAuth);

    mUnits = new QMenu("Units");
    mUnits->addAction("Add unit...");
    mUnits->addAction("Change unit...");

    m->addMenu(mUnits);
    m->addAction("Settings...");

    this->setMenuBar(m);

    leftDockWidget = new QDockWidget(this);
    leftDockWidget->setObjectName("leftDockWidget");
    sensors_view = new QListView;
    placesBox = new QComboBox;
    unitsBox = new QComboBox;
//    unitsBox->addItems(QStringList()<< "unit1" << "unit2" << "unit3");
    bufleftDockWidget = new QWidget;
    leftDockVBL = new QVBoxLayout;
    leftDockVBL->addWidget(new QLabel("Enter units location: "));
    leftDockVBL->addWidget(placesBox);
    leftDockVBL->addWidget(new QLabel("Enter unit: "));
    leftDockVBL->addWidget(unitsBox);
    leftDockVBL->addWidget(sensors_view);
    bufleftDockWidget->setLayout(leftDockVBL);

    leftDockWidget->setWidget(bufleftDockWidget);
    leftDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);


    this->addDockWidget(Qt::LeftDockWidgetArea, leftDockWidget);

    QLabel *centralWidget = new QLabel(tr("Central widget")); // TODO: графики графики
    centralWidget->setAlignment(Qt::AlignCenter);
    setCentralWidget(centralWidget);

    connect(aAuth, &QAction::triggered, wAuth, &AuthorizeWindow::show);
    connect(wAuth, &AuthorizeWindow::signalUserLogined, this, &MainWindow::slotSaveDataFromAuth);
    connect(m, &QMenuBar::triggered, this, &MainWindow::slotMenuTriggered);
    connect(wSettings, &settingsWindow::signalSettingsChanged, this, &MainWindow::slotWSeetingsChanged);

    setWindowTitle("RMonitoring: unauthorize");

    setMinimumSize(700, 550);
}

MainWindow::~MainWindow() {

}

void MainWindow::slotSaveDataFromAuth() {
    setWindowTitle("RMonitoring: "+wAuth->get_name());
    jwt_token = wAuth->get_jwt_token();
    jwt_refresh_token = wAuth->get_jwt_refresh_token();
}

void MainWindow::slotMenuTriggered(QAction *a) {
    QString actionText = a->text();

    if(actionText == "Settings..."){
        wSettings->show();
    }
}

void MainWindow::slotWSeetingsChanged() {
    url = wSettings->getUrl();
}

