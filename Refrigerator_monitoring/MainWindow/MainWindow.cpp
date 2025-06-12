#include "MainWindow/MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), settings("HladCode", "RMonitorig") {
    manager = new QNetworkAccessManager(this);
    errMsg = new QErrorMessage;

    series = new QLineSeries();
    series->setName("Sensor Data");

    chart = new QChart();
    chart->setTitle("Sensor Value over Time");
    chart->legend()->hide();
    chart->addSeries(series);

    axisX = new QDateTimeAxis;
    axisX->setFormat("HH:mm");
    axisX->setTitleText("Time");

    axisY = new QValueAxis;
    axisY->setTitleText("Sensor Value");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chartView = new ChartView(chart, series);
    chartInfo = new QLabel("Chart");
    chartView->setRenderHint(QPainter::Antialiasing);


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
    bApplyFilter = new QPushButton("Show sensors");
    startDateTimeEdit = new QDateTimeEdit(this);
    endDateTimeEdit = new QDateTimeEdit(this);
    startDateTimeEdit->setCalendarPopup(true);
    startDateTimeEdit->setDisplayFormat("dd-MM-yyyy HH:mm");
    startDateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endDateTimeEdit->setCalendarPopup(true);
    endDateTimeEdit->setDisplayFormat("dd-MM-yyyy HH:mm");
    endDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    endDateTimeEdit->setMinimumDateTime(startDateTimeEdit->dateTime());


    connect(startDateTimeEdit, &QDateTimeEdit::timeChanged, this, &MainWindow::slotStartTimeChanged);
    connect(endDateTimeEdit, &QDateTimeEdit::timeChanged, this, &MainWindow::slotEndTimeChanged);

    isRealTime = new QCheckBox;
    //bStartRealTime = new QPushButton("Start real time");
    //bStartRealTime->setDisabled(true);
    isRealTime->setCheckState(Qt::CheckState::Unchecked);
    realTimeTimer = new QTimer(this);
    realTimeTimer->setInterval(60000); // кожну 1 хвилину (60 000 мс) TODO: зробити в налаштуваннях задання інтервалу

    connect(realTimeTimer, &QTimer::timeout, this, &MainWindow::slotUpdateRealTimeData);
    connect(isRealTime, &QCheckBox::checkStateChanged, this, &MainWindow::slotIsRealTimeChecked);


    QHBoxLayout* hblDateTime1 = new QHBoxLayout;
    QHBoxLayout* hblDateTime2 = new QHBoxLayout;
    QHBoxLayout* hblDateTime3 = new QHBoxLayout;
    hblDateTime1->addWidget(new QLabel("Start time: "));
    hblDateTime1->addWidget(startDateTimeEdit);
    hblDateTime2->addWidget(new QLabel("End time: "));
    hblDateTime2->addWidget(endDateTimeEdit);
    hblDateTime3->addWidget(new QLabel("Enable real time:"));
    hblDateTime3->addWidget(isRealTime);

//-----------------------------------
    bufleftDockWidget = new QWidget;
    leftDockVBL = new QVBoxLayout;
    leftDockVBL->addWidget(new QLabel("Enter units location: "));
    leftDockVBL->addWidget(placesBox);
    leftDockVBL->addWidget(new QLabel("Enter unit: "));
    leftDockVBL->addWidget(unitsBox);
    leftDockVBL->addWidget(bApplyFilter);
    leftDockVBL->addWidget(sensors_view);
    leftDockVBL->addLayout(hblDateTime1);
    leftDockVBL->addLayout(hblDateTime2);
    leftDockVBL->addWidget(bGetSensorData);
    leftDockVBL->addSpacing(15);
    leftDockVBL->addLayout(hblDateTime3);
    //leftDockVBL->addWidget(bStartRealTime);
    bufleftDockWidget->setLayout(leftDockVBL);

    leftDockWidget->setWidget(bufleftDockWidget);
    leftDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);


    this->addDockWidget(Qt::LeftDockWidgetArea, leftDockWidget);

//----------------
    QWidget* Central_widget = new QWidget;
    QVBoxLayout* centralVBL = new QVBoxLayout;
    QHBoxLayout* centralHBL1 = new QHBoxLayout;

    QPushButton* bXPlus = new QPushButton("+");
    QPushButton* bXMinus = new QPushButton("-");
    centralHBL1->addWidget(new QLabel("Change x scale: ")); // TODO:  with interval
    centralHBL1->addWidget(bXMinus);
    centralHBL1->addWidget(bXPlus);

    centralVBL->addWidget(chartView);
    centralVBL->addWidget(chartInfo);
    centralVBL->addLayout(centralHBL1);

    Central_widget->setLayout(centralVBL);
    connect(bXPlus, &QPushButton::clicked, this, &MainWindow::slotPlusAxisX);
    connect(bXMinus, &QPushButton::clicked, this, &MainWindow::slotMinusAxisX);

    setCentralWidget(Central_widget);
//----------------
    connect(aAuth, &QAction::triggered, wAuth, &AuthorizeWindow::show);
    connect(wAuth, &AuthorizeWindow::signalUserLogined, this, &MainWindow::slotSaveDataFromAuth);
    connect(m, &QMenuBar::triggered, this, &MainWindow::slotMenuTriggered);
    connect(wSettings, &settingsWindow::signalSettingsChanged, this, &MainWindow::slotWSeetingsChanged);
    connect(placesBox, &QComboBox::currentTextChanged, this, &MainWindow::slotPlacesBox);
    connect(bApplyFilter, &QPushButton::clicked, this, &MainWindow::slotApplyFilterClicked);
    connect(bGetSensorData, &QPushButton::released, this, &MainWindow::slotGetSensorData);
    connect(series, &QLineSeries::hovered, this, [=](const QPointF &point, bool state) {
        if (state) {
            QDateTime dt = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(std::round(point.x())));
            //dt.setTimeSpec(Qt::UTC);
            //dt = dt.toTimeZone(QTimeZone("+03:00"));

            QString timeText = dt.toString(Qt::ISODate);
            QString valueText = QString("Value: %1").arg(point.y(), 0, 'f', 2);
            timeText.replace('T', ' ');
            QString fullText = "Date time: " + timeText + "\n" + valueText;

            qDebug() << "Final text:" << fullText;

            chartInfo->setText(fullText);
            QToolTip::showText(QCursor::pos(), fullText);

        } else {
            QToolTip::hideText();
        }
    });


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
                unitList->addItem("Sensor id: "+key+" "+value, key.toInt());
            }
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void MainWindow::slotStartTimeChanged() { }

void MainWindow::slotEndTimeChanged() { }

void MainWindow::slotPlusAxisX() {
    QDateTime min = axisX->min().addSecs(3600); // минус 1 час
    QDateTime max = axisX->max().addSecs(-3600);  // плюс 1 час
    axisX->setRange(min, max);
}

void MainWindow::slotMinusAxisX() {
    QDateTime min = axisX->min().addSecs(-3600); // минус 1 час
    QDateTime max = axisX->max().addSecs(3600);  // плюс 1 час
    axisX->setRange(min, max);
}

void MainWindow::slotIsRealTimeChecked(Qt::CheckState state) {
    if(state == Qt::CheckState::Checked) {
        int n = QMessageBox::warning(0, "Warning",
                                     "Do you really want to start real time monitoring from date time "+
                                     startDateTimeEdit->dateTime().toString("yyyy-MM-ddTHH mm:ss"),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(n == QMessageBox::No){
            isRealTime->setCheckState(Qt::CheckState::Unchecked);
            return;
        }

        endDateTimeEdit->setDisabled(true);
        bGetSensorData->setDisabled(true);
        //bStartRealTime->setDisabled(false);

        realTimeTimer->start(); // запускаємо таймер
        slotGetSensorData(); // викликаємо перший запит одразу

    } else {
        endDateTimeEdit->setDisabled(false);
        bGetSensorData->setDisabled(false);
        //bStartRealTime->setDisabled(true);

        realTimeTimer->stop();
    }
}

// TODO: вынести эти две хуйни по запросу даты с сервака в одну функцию
// и сделать еще рефреш токенов, если там хуйня выходит ок да
void MainWindow::slotUpdateRealTimeData() {
    if(jwt_token.isEmpty()) return;
    //qDebug() << unitList->sensor_index(sensors_view->currentIndex());
    QNetworkRequest request(QUrl(url+"/user/getDataInInterval"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + jwt_token).toUtf8());

    QJsonObject json;
    json["ID"] = unitsBox->currentText();
    json["sensor_ID"] = unitList->sensor_index(sensors_view->currentIndex());
    json["from"] = startDateTimeEdit->dateTime().toString("yyyy-MM-ddTHH:mm:ss")+("+03:00");
    json["to"] = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss")+("+03:00");

    QNetworkReply *reply = manager->post(QNetworkRequest(request), QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        QByteArray response = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Response:" << response;
            if(response.contains("Invalid token") == true) {
                QMessageBox::information(0, "Information", "You are not logined: " + response);
                reply->deleteLater();
                return;
            }

            //response.remove(response.length()-4, 4);

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << parseError.errorString();
                return;
            }

            if (!doc.isObject()) {
                qDebug() << "JSON is not an object!";
                return;
            }
            QJsonObject obj = doc.object();

            if(obj.isEmpty()) {
                QMessageBox::information(0, "Information", "There are no data in response(");
                return;
            }

            series->clear();
            chart->removeAxis(axisX);
            chart->removeAxis(axisY);

            axisX = new QDateTimeAxis;
            axisX->setFormat("dd-MM HH:mm");
            axisX->setTitleText("Time");
            axisX->setTickCount(10);

            axisY = new QValueAxis;
            axisY->setTitleText("Sensor value");

            // Обработать JSON
            QMap<QDateTime, double> sortedData;
            qint64 minTime = LLONG_MAX;
            qint64 maxTime = LLONG_MIN;

            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QDateTime dt = QDateTime::fromString(it.key(), Qt::ISODate);
                if (!dt.isValid()) {
                    qDebug() << "Invalid datetime:" << it.key();
                    continue;
                }
                dt.setTimeSpec(Qt::OffsetFromUTC);
                qint64 ms = dt.toMSecsSinceEpoch();

                if (ms < minTime) minTime = ms;
                if (ms > maxTime) maxTime = ms;

                sortedData[dt] = it.value().toDouble();
            }

            // Добавить точки на график
            for (auto it = sortedData.begin(); it != sortedData.end(); ++it) {
                series->append(it.key().toMSecsSinceEpoch(), it.value());
            }

            // Добавить серию на график
            chart->addSeries(series);
            chart->addAxis(axisX, Qt::AlignBottom);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisX);
            series->attachAxis(axisY);

            // Настроить диапазоны осей
            axisX->setMin(QDateTime::fromMSecsSinceEpoch(minTime));
            axisX->setMax(QDateTime::fromMSecsSinceEpoch(maxTime));

            // (опционально) Автоматически масштабировать Y-ось
            double minY = std::numeric_limits<double>::max();
            double maxY = std::numeric_limits<double>::lowest();
            for (const auto& v : sortedData.values()) {
                if (v < minY) minY = v;
                if (v > maxY) maxY = v;
            }
            axisY->setRange(minY-1, maxY+1);

            series->setColor(Qt::blue);
            series->setPointsVisible(true);                // Показывать точки
            series->setPointLabelsVisible(true);           // Показывать подписи
            series->setPointLabelsFormat("@yPoint");       // Формат: только Y-значение
            series->setPointLabelsClipping(false);         // Разрешить подписи выходить за границы
            series->setPointLabelsColor(Qt::black);        // Цвет текста
            series->setPointLabelsFont(QFont("Arial", 8)); // Размер шрифта


        } else {
            QMessageBox::information(0, "Information", "NETWORK ERROR");
        }
        reply->deleteLater();
    });
}

void MainWindow::slotGetSensorData() {
    if(jwt_token.isEmpty()) return;
    //qDebug() << unitList->sensor_index(sensors_view->currentIndex());
    QNetworkRequest request(QUrl(url+"/user/getDataInInterval"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + jwt_token).toUtf8());

    QJsonObject json;
    json["ID"] = unitsBox->currentText();
    json["sensor_ID"] = unitList->sensor_index(sensors_view->currentIndex());
    json["from"] = startDateTimeEdit->dateTime().toString("yyyy-MM-ddTHH:mm:ss")+("+03:00");
    json["to"] = endDateTimeEdit->dateTime().toString("yyyy-MM-ddTHH:mm:ss")+("+03:00");

    QNetworkReply *reply = manager->post(QNetworkRequest(request), QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        QByteArray response = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Response:" << response;
            if(response.contains("Invalid token") == true) {
                QMessageBox::information(0, "Information", "You are not logined: " + response);
                reply->deleteLater();
                return;
            }

            //response.remove(response.length()-4, 4);

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "JSON parse error:" << parseError.errorString();
                return;
            }

            if (!doc.isObject()) {
                qDebug() << "JSON is not an object!";
                return;
            }
            QJsonObject obj = doc.object();

            if(obj.isEmpty()) {
                QMessageBox::information(0, "Information", "There are no data in response(");
                return;
            }

            series->clear();
            chart->removeAxis(axisX);
            chart->removeAxis(axisY);

            axisX = new QDateTimeAxis;
            axisX->setFormat("dd-MM HH:mm");
            axisX->setTitleText("Time");
            axisX->setTickCount(10);

            axisY = new QValueAxis;
            axisY->setTitleText("Sensor value");

            // Обработать JSON
            QMap<QDateTime, double> sortedData;
            qint64 minTime = LLONG_MAX;
            qint64 maxTime = LLONG_MIN;

            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QDateTime dt = QDateTime::fromString(it.key(), Qt::ISODate);
                if (!dt.isValid()) {
                    qDebug() << "Invalid datetime:" << it.key();
                    continue;
                }
                dt.setTimeSpec(Qt::OffsetFromUTC);
                qint64 ms = dt.toMSecsSinceEpoch();

                if (ms < minTime) minTime = ms;
                if (ms > maxTime) maxTime = ms;

                sortedData[dt] = it.value().toDouble();
            }

            // Добавить точки на график
            for (auto it = sortedData.begin(); it != sortedData.end(); ++it) {
                series->append(it.key().toMSecsSinceEpoch(), it.value());
            }

            // Добавить серию на график
            chart->addSeries(series);
            chart->addAxis(axisX, Qt::AlignBottom);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisX);
            series->attachAxis(axisY);

            // Настроить диапазоны осей
            axisX->setMin(QDateTime::fromMSecsSinceEpoch(minTime));
            axisX->setMax(QDateTime::fromMSecsSinceEpoch(maxTime));

            // (опционально) Автоматически масштабировать Y-ось
            double minY = std::numeric_limits<double>::max();
            double maxY = std::numeric_limits<double>::lowest();
            for (const auto& v : sortedData.values()) {
                if (v < minY) minY = v;
                if (v > maxY) maxY = v;
            }
            axisY->setRange(minY-1, maxY+1);

            series->setColor(Qt::blue);
            series->setPointsVisible(true);                // Показывать точки
            series->setPointLabelsVisible(true);           // Показывать подписи
            series->setPointLabelsFormat("@yPoint");       // Формат: только Y-значение
            series->setPointLabelsClipping(false);         // Разрешить подписи выходить за границы
            series->setPointLabelsColor(Qt::black);        // Цвет текста
            series->setPointLabelsFont(QFont("Arial", 8)); // Размер шрифта


        } else {
            QMessageBox::information(0, "Information", "NETWORK ERROR");
        }
        reply->deleteLater();
    });
}



