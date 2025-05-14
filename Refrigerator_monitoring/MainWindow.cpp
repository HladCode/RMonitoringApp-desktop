#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    m = new QMenuBar;

    QPixmap pix("debug\\icons\\Account.png");
    QPixmap scaled = pix.scaled(55, 55, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QIcon icon;
    icon.addPixmap(scaled);
    aAuth = new QAction(icon, "Login");
    wAuth = new AuthorizeWindow(this);
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

    setWindowTitle("RMonitoring: unauthorize");
}

MainWindow::~MainWindow() {

}

