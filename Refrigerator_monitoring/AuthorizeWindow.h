#ifndef AUTHORIZEWINDOW_H
#define AUTHORIZEWINDOW_H

#include <QtWidgets>

class AuthorizeWindow : public QWidget
{
    Q_OBJECT

    QLineEdit* name;
    QLineEdit* password;
    QPushButton* bAuthorize;

    QVBoxLayout *vbl;

public:
    explicit AuthorizeWindow(QWidget *parent = nullptr);

signals:

};

#endif // AUTHORIZEWINDOW_H
