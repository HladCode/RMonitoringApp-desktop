#include "MainWindow/MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), settings("HladCode", "RMonitorig") {
    m = new QMenuBar;

    aAuth = new QAction("Login");
    wAuth = new AuthorizeWindow("http://78.137.37.34:1488");
    m->addAction(aAuth);

    this->setMenuBar(m);

    leftDockWidget = new QDockWidget(tr("Sensors"), this);
    leftDockWidget->setObjectName("leftDockWidget");
    sensors_view = new QListView;
    leftDockWidget->setWidget(sensors_view);
    leftDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);


    this->addDockWidget(Qt::LeftDockWidgetArea, leftDockWidget);

    QLabel *centralWidget = new QLabel(tr("Central widget")); // TODO: графики графики
    centralWidget->setAlignment(Qt::AlignCenter);
    setCentralWidget(centralWidget);

    connect(aAuth, &QAction::triggered, wAuth, &AuthorizeWindow::show);
    connect(wAuth, &AuthorizeWindow::signalUserLogined, this, &MainWindow::slotSaveDataFromAuth);

    setWindowTitle("RMonitoring: unauthorize");

    setMinimumSize(700, 550);
}

MainWindow::~MainWindow() {

}

void MainWindow::slotSaveDataFromAuth() {
    setWindowTitle("RMonitoring: "+wAuth->get_name());
}

