#ifndef AUTHORIZEWINDOW_H
#define AUTHORIZEWINDOW_H

#include <QtWidgets>
#include <QtNetwork>

class AuthorizeWindow : public QWidget
{
    Q_OBJECT

    QLineEdit* name;
    QLineEdit* password;
    QPushButton* bAuthorize;
    QTextEdit* statusText;

    QVBoxLayout *vbl;

    QString url;
    QString jwt_token;
    QString jwt_refresh_token;

    QNetworkAccessManager *manager;

public:
    explicit AuthorizeWindow(QString url, QWidget *parent = nullptr);

    void set_url(QString& url);

    QString get_name();
    QString get_jwt_token();
    QString get_jwt_refresh_token();
protected slots:
    void slotAuthorizeButtonClicked();
protected: signals:
    void signalUserLogined();
};

#endif // AUTHORIZEWINDOW_H
