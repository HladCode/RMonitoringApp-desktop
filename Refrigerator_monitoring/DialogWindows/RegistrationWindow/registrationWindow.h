#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include "qnetworkaccessmanager.h"
#include "lib/CaptchaWidget/captchaWidget.h"
#include <QtNetwork>
#include <QtWidgets>

class RegistrationWindow : public QWidget
{
    QLineEdit* name;
    QLineEdit* password;
    QLineEdit* verifyPassword;
    QLineEdit* email;
    CaptchaWidget* captcha;
    QTextEdit* statusText;

    QVBoxLayout *vbl;

    QString url;
    QNetworkAccessManager *manager;

public:
    explicit RegistrationWindow(QString url, QWidget *parent = nullptr);

    void set_url(QString& url);

protected slots:
    void slotAuthorizeButtonClicked();
//private: signals:
//    void signalUserRegister();
};

#endif // REGISTRATIONWINDOW_H
