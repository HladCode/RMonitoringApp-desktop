#include "AuthorizeWindow.h"

AuthorizeWindow::AuthorizeWindow(QWidget *anotherParent, QWidget *parent) : QWidget{parent} {
    this->anotherParent = &anotherParent;
    vbl = new QVBoxLayout;
    name = new QLineEdit;
    password = new QLineEdit;
    bAuthorize = new QPushButton("Authorize");

    name->setPlaceholderText("...");
    password->setPlaceholderText("...");

    vbl->addWidget(new QLabel("Enter account name"));
    vbl->addWidget(name);
    vbl->addWidget(new QLabel("Enter password"));
    vbl->addWidget(password);
    vbl->addWidget(bAuthorize);

    setLayout(vbl);

    setWindowTitle("Authorize");

    connect(bAuthorize, &QPushButton::clicked, this, &AuthorizeWindow::slotAuthorizeButtonClicked);
}

void AuthorizeWindow::slotAuthorizeButtonClicked(){
    (*anotherParent)->setWindowTitle("RMonitoring: "+name->text()); // TODO: доделать эту хуйню как-то через вызов сигналов и без anotherParent
}

