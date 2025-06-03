#include "settingsWindow.h"

settingsWindow::settingsWindow(QWidget *parent): QWidget(parent) {
    urlChange = new QLineEdit;

    QVBoxLayout *vbl = new QVBoxLayout;
    vbl->addWidget(new QLabel("Your url: "));
    vbl->addWidget(urlChange);

    QPushButton* bApply = new QPushButton("Apply changes");
    QPushButton* bDecline = new QPushButton("Decline changes");
    QHBoxLayout* hbl = new QHBoxLayout;
    hbl->addWidget(bApply);
    hbl->addWidget(bDecline);
    vbl->addLayout(hbl);

    setLayout(vbl);

    connect(bApply, &QPushButton::clicked, this, &settingsWindow::SlotBApplyClicked);
}

QString settingsWindow::getUrl() {
    return urlChange->text();
}

void settingsWindow::setUrl(const QString &url) {
    this->urlChange->setText(url);
}

void settingsWindow::SlotBApplyClicked() {
    emit this->signalSettingsChanged(); // TODO: make some proper settings apply logic
    close();
}
