#include "registrationWindow.h"



RegistrationWindow::RegistrationWindow(QString url, QWidget *parent) : QWidget{parent} {
    this->url = url;
    vbl = new QVBoxLayout;
    name = new QLineEdit;
    password = new QLineEdit;
    verifyPassword = new QLineEdit;
    email = new QLineEdit;
    captcha = new CaptchaWidget("Register");
    statusText = new QTextEdit("Please, enter login & password of your account, \nthat was verified by your company");

    manager = new QNetworkAccessManager(this);

    name->setPlaceholderText("...");
    password->setPlaceholderText("...");
    statusText->setReadOnly(true);

    vbl->addWidget(new QLabel("Enter account name"));
    vbl->addWidget(name);
    vbl->addWidget(new QLabel("Enter your email"));
    vbl->addWidget(email);
    vbl->addWidget(new QLabel("Enter password"));
    vbl->addWidget(password);
    vbl->addWidget(new QLabel("Verify password"));
    vbl->addWidget(verifyPassword);
    vbl->addWidget(captcha);
    vbl->addStretch(0);
    vbl->addWidget(statusText);

    setLayout(vbl);

    setWindowTitle("Authorize");

    connect(captcha, &CaptchaWidget::signalCaptchaSuccess, this, &RegistrationWindow::slotAuthorizeButtonClicked);

    setFixedWidth(350);
    setMinimumHeight(210);
}

void RegistrationWindow::set_url(QString &url)
{
    this->url = url;
}

void RegistrationWindow::slotAuthorizeButtonClicked(){
    if(password->text() != verifyPassword->text()) {
        statusText->setText("Passwords are not eaqual");
        return;
    }

    QNetworkRequest request(QUrl(url+"/auth/register"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["Login"] = name->text();
    json["Email"] = email->text();
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

            //emit signalUserRegister();
            QMessageBox::information(0, "Information", QString("You are going to get access to data of your company after company administrator")+
                                                       QString(" add you to their RMonitoring members. Login, after you get access."));
            this->close();

        } else {
            statusText->setText("NETWORK ERROR");
        }
        reply->deleteLater();
    });
}
