#include "MainWindow/MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), settings("HladCode", "RMonitorig") {
    manager = new QNetworkAccessManager(this);
    errMsg = new QErrorMessage;

    m = new QMenuBar;

    aAuth = new QAction("Login...");
    wSettings = new settingsWindow;
    wSettings->setUrl("http://78.137.37.34:1488"); // TODO: make default url be from QSettings by setURL
    url = wSettings->getUrl();
    wAuth = new AuthorizeWindow(url);
    wReg = new RegistrationWindow(url);

    QMenu* mAuth = new QMenu("Authorization");
    mAuth->addAction(aAuth);
    mAuth->addAction("Register...");


    mUnits = new QMenu("Units");
    mUnits->addAction("Add unit...");
    mUnits->addAction("Change unit...");

    mUsers = new QMenu("Users");
    mUsers->addAction("Add user to company...");
    mUsers->addAction("Change user...");

    m->addMenu(mAuth);
    m->addMenu(mUnits);
    m->addMenu(mUsers);
    m->addAction("Settings...");

    this->setMenuBar(m);

    leftDockWidget = new QDockWidget(this);
    leftDockWidget->setObjectName("leftDockWidget");
    unitList = new unitModel;
    sensors_view = new QListView;
    sensors_view->setSpacing(5);
    sensors_view->setModel(unitList);
    bGetSensorData = new QPushButton("Get sensor data");
    placesBox = new QComboBox;
    unitsBox = new QComboBox;
//----------------------------------
    dateData = new QDateEdit(QDate::currentDate());
    dateData->setDisplayFormat("yyyy MM dd");

    bApplyFilter = new QPushButton("Show sensors");
    QTimeEdit* startTimeEdit = new QTimeEdit(this);
    QTimeEdit* endTimeEdit = new QTimeEdit(this);
    startTimeEdit->setTime(QTime(0,0));
    endTimeEdit->setTime(QTime(23,59));
    startTimeEdit->setDisplayFormat("HH:mm");
    endTimeEdit->setDisplayFormat("HH:mm");
    connect(startTimeEdit, &QTimeEdit::timeChanged, this, &MainWindow::slotStartTimeChanged);
    connect(endTimeEdit, &QTimeEdit::timeChanged, this, &MainWindow::slotEndTimeChanged);
    QHBoxLayout* hblTimeEdits = new QHBoxLayout;
    hblTimeEdits->addWidget(new QLabel("Start time: "));
    hblTimeEdits->addWidget(startTimeEdit);
    hblTimeEdits->addWidget(new QLabel("End time: "));
    hblTimeEdits->addWidget(endTimeEdit);

//-----------------------------------
    bufleftDockWidget = new QWidget;
    leftDockVBL = new QVBoxLayout;
    leftDockVBL->addWidget(new QLabel("Enter units location: "));
    leftDockVBL->addWidget(placesBox);
    leftDockVBL->addWidget(new QLabel("Enter unit: "));
    leftDockVBL->addWidget(unitsBox);
    leftDockVBL->addWidget(bApplyFilter);
    leftDockVBL->addWidget(sensors_view);
    leftDockVBL->addWidget(dateData);
    leftDockVBL->addLayout(hblTimeEdits);
    leftDockVBL->addWidget(bGetSensorData);
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
    connect(placesBox, &QComboBox::currentTextChanged, this, &MainWindow::slotPlacesBox);
    connect(bApplyFilter, &QPushButton::clicked, this, &MainWindow::slotApplyFilterClicked);
    connect(bGetSensorData, &QPushButton::released, this, &MainWindow::slotGetSensorData);

    setWindowTitle("RMonitoring: unauthorize");

    setMinimumSize(700, 550);
}

MainWindow::~MainWindow() {

}

void MainWindow::getUnits() {
    QUrl url_object(url+"/user/getDevices");
    QNetworkRequest request(url_object);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + jwt_token).toUtf8());

    QJsonObject json;
    json["username"] = wAuth->get_name();

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // POST-запрос
    QNetworkReply *reply = manager->post(request, data);

    // Обработка ответа
    QObject::connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response_data = reply->readAll();
            qDebug() << response_data << "\n";
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(response_data, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << parseError.errorString();
                reply->deleteLater();
                return;
            }

            if (!doc.isObject()) {
                qDebug() << "Expected JSON object";
                reply->deleteLater();
                return;
            }

            QJsonObject rootObj = doc.object();

            units.clear();
            placesBox->clear();
            unitsBox->clear();

            for (auto it = rootObj.begin(); it != rootObj.end(); ++it) {
                QString key = it.key();
                placesBox->addItem(key);
                QJsonArray arr = it.value().toArray();

                QStringList values;
                for (const QJsonValue &val : arr) {
                    values.append(val.toString());
                }

                qDebug() << "1: " << key << values;
                units.insert(key, values);
            }

            placesBox->addItem("...");

//            slotPlacesBox(0);

//            if (this->placesBox->count() > 0){
//                slotPlacesBox(0);
//            } else {
//                this->errMsg->showMessage("Something wrong with data(");
//            }

        } else {
            this->errMsg->showMessage("Can not get units data: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void MainWindow::slotSaveDataFromAuth() {
    setWindowTitle("RMonitoring: "+wAuth->get_name());
    jwt_token = wAuth->get_jwt_token();
    jwt_refresh_token = wAuth->get_jwt_refresh_token();

    getUnits();
}

void MainWindow::slotMenuTriggered(QAction *a) {
    QString actionText = a->text();

    if(actionText == "Settings..."){
        wSettings->show();
    } else if (actionText == "Register...") {
        wReg->show();
    }
}

void MainWindow::slotWSeetingsChanged() {
    url = wSettings->getUrl();
    wAuth->set_url(url);
}

void MainWindow::slotPlacesBox(const QString &place) {
    if(place == "...") return;
    unitsBox->clear();
    unitsBox->addItems(units[place]);
}

void MainWindow::slotApplyFilterClicked() {
    QString device_id = unitsBox->currentText();

    QUrl url_object(url+"/user/getSensors");
    QNetworkRequest request(url_object);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + jwt_token).toUtf8());

    QJsonObject json;
    json["Device_id"] = device_id;
    QJsonDocument jsonDoc(json);
    QByteArray postData = jsonDoc.toJson();

    QNetworkReply *reply = manager->post(request, postData);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << parseError.errorString();
                reply->deleteLater();
                return;
            }

            if (!doc.isObject()) {
                qDebug() << "Expected JSON object.";
                reply->deleteLater();
                return;
            }

            QJsonObject obj = doc.object();

            unitList->clear();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QString key = it.key();
                QString value = it.value().toString();
                unitList->addItem("Sensor id: "+key+" "+value);
            }
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void MainWindow::slotGetSensorData()
{

}

void MainWindow::slotStartTimeChanged() {

}

void MainWindow::slotEndTimeChanged()
{

}

