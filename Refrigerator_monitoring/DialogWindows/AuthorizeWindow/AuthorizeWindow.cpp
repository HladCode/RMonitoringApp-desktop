#include "AuthorizeWindow.h"

AuthorizeWindow::AuthorizeWindow(QString url, QWidget *parent) : QWidget{parent} {
    this->url = url;
    vbl = new QVBoxLayout;
    name = new QLineEdit;
    password = new QLineEdit;
    bAuthorize = new QPushButton("Authorize");
    statusText = new QTextEdit("Please, enter login & password of your account, \nthat was verified by your company");

    manager = new QNetworkAccessManager(this);

    name->setPlaceholderText("...");
    password->setPlaceholderText("...");
    statusText->setReadOnly(true);

    vbl->addWidget(new QLabel("Enter account name"));
    vbl->addWidget(name);
    vbl->addWidget(new QLabel("Enter password"));
    vbl->addWidget(password);
    vbl->addWidget(bAuthorize);
    vbl->addStretch(0);
    vbl->addWidget(statusText);

    setLayout(vbl);

    setWindowTitle("Authorize");

    connect(bAuthorize, &QPushButton::clicked, this, &AuthorizeWindow::slotAuthorizeButtonClicked);

    setFixedWidth(350);
    setMinimumHeight(210);
}

void AuthorizeWindow::set_url(QString &url)
{
    this->url = url;
}

QString AuthorizeWindow::get_name()
{
    return name->text();
}

QString AuthorizeWindow::get_jwt_token()
{
    return this->jwt_token;
}

QString AuthorizeWindow::get_jwt_refresh_token()
{
    return this->jwt_refresh_token;
}

void AuthorizeWindow::slotAuthorizeButtonClicked(){

    QNetworkRequest request(QUrl(url+"/auth/login"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["Login"] = name->text();
    json["password"] = password->text();

    QNetworkReply *reply = manager->post(QNetworkRequest(request), QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        QByteArray response = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Response:" << response;
            if(response.endsWith("200") == false) {
                statusText->setText("Can not login: " + response);
                reply->deleteLater();
                return;
            }

            response.remove(response.length()-4, 4);

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

            QString accessToken = obj["access_token"].toString();
            QString refreshToken = obj["refresh_token"].toString();

            qDebug() << "Access token:" << accessToken;
            qDebug() << "Refresh token:" << refreshToken;

            this->jwt_token = accessToken;
            this->jwt_refresh_token = refreshToken;

            emit this->signalUserLogined();
            this->close();

        } else {
            statusText->setText("NETWORK ERROR");
        }
        reply->deleteLater();
    });



}

