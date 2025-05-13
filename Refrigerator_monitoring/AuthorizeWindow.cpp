#include "AuthorizeWindow.h"

AuthorizeWindow::AuthorizeWindow(QWidget *parent) : QWidget{parent} {
    vbl = new QVBoxLayout;
    name = new QLineEdit(this);
    password = new QLineEdit;
    bAuthorize = new QPushButton("Authorize");


    vbl->addWidget(name);
    vbl->addWidget(password);
    vbl->addWidget(bAuthorize);

    setLayout(vbl);
}

