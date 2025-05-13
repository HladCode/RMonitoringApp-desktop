#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    m = new QMenuBar;

   m->addAction("");

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
}

MainWindow::~MainWindow()
{
}

