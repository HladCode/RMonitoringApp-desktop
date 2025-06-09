#include "captchaWidget.h"

CaptchaWidget::CaptchaWidget(const QString& button_name, QWidget *parent) : QWidget(parent) {
    captchaLabel = new QLabel(this);
    input = new QLineEdit(this);
    checkButton = new QPushButton(button_name, this);
    resultLabel = new QLabel(this);

    input->setPlaceholderText("Enter captcha here");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(captchaLabel);
    layout->addWidget(input);
    layout->addWidget(checkButton);
    layout->addWidget(resultLabel);
    setLayout(layout);

    connect(checkButton, &QPushButton::clicked, this, &CaptchaWidget::verifyCaptcha);

    refreshCaptcha();
}

QString CaptchaWidget::generateCaptchaText(int length) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    QString captcha;
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        captcha.append(possibleCharacters.at(index));
    }
    return captcha;
}

QPixmap CaptchaWidget::drawCaptchaImage(const QString& text, QSize size) {
    QPixmap pixmap(size);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    QFont font("Arial", 24, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, text);

    for (int i = 0; i < 10; ++i) {
        QPen pen(QColor::fromRgb(QRandomGenerator::global()->generate()));
        painter.setPen(pen);
        int x1 = QRandomGenerator::global()->bounded(size.width());
        int y1 = QRandomGenerator::global()->bounded(size.height());
        int x2 = QRandomGenerator::global()->bounded(size.width());
        int y2 = QRandomGenerator::global()->bounded(size.height());
        painter.drawLine(x1, y1, x2, y2);
    }

    return pixmap;
}

void CaptchaWidget::refreshCaptcha() {
    currentCaptcha = generateCaptchaText();
    captchaLabel->setPixmap(drawCaptchaImage(currentCaptcha));
    input->clear();
    //resultLabel->clear();
}

void CaptchaWidget::verifyCaptcha() {
    QString userInput = input->text().toUpper();
    if (userInput == currentCaptcha) {
        emit signalCaptchaSuccess();
        refreshCaptcha();
    } else {
        resultLabel->setText("Captcha is not correct");
        refreshCaptcha();
    }
}
