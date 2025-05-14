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

    QWidget** anotherParent;

public:
    explicit AuthorizeWindow(QWidget *anotherParent, QWidget *parent = nullptr);

protected slots:
    void slotAuthorizeButtonClicked();
};

#endif // AUTHORIZEWINDOW_H
