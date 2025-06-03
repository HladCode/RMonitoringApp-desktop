#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtWidgets>

class settingsWindow : public QWidget
{
    Q_OBJECT

    QLineEdit* urlChange;


public:
    settingsWindow(QWidget *parent = nullptr);

    QString getUrl();
    void setUrl(const QString& url);

public: signals:
    void signalSettingsChanged();
protected slots:
    void SlotBApplyClicked();
};

#endif // SETTINGSWINDOW_H
