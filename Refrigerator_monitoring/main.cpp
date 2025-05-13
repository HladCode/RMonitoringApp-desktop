#include "AuthorizeWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthorizeWindow *w = new AuthorizeWindow;
    w->show();
    return a.exec();
}
