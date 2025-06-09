QT       += core gui
QT += network
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DialogWindows/AuthorizeWindow/AuthorizeWindow.cpp \
    DialogWindows/RegistrationWindow/registrationWindow.cpp \
    DialogWindows/SettingsWindow/settingsWindow.cpp \
    MainWindow/unitListModel/unitListModel.cpp \
    lib/CaptchaWidget/captchaWidget.cpp \
    main.cpp \
    MainWindow/MainWindow.cpp

HEADERS += \
    DialogWindows/AuthorizeWindow/AuthorizeWindow.h \
    DialogWindows/RegistrationWindow/registrationWindow.h \
    DialogWindows/SettingsWindow/settingsWindow.h \
    MainWindow/MainWindow.h \
    MainWindow/unitListModel/unitListModel.h \
    lib/CaptchaWidget/captchaWidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
