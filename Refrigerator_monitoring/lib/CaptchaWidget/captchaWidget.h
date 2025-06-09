#ifndef CAPTCHAWIDGET_H
#define CAPTCHAWIDGET_H

#include <QtWidgets>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QRandomGenerator>

class CaptchaWidget : public QWidget {
    Q_OBJECT

    QLabel* captchaLabel;
    QLineEdit* input;
    QPushButton* checkButton;
    QLabel* resultLabel;

    QString currentCaptcha;
public:
    explicit CaptchaWidget(const QString& button_name, QWidget *parent = nullptr);

protected slots:
    void verifyCaptcha();

protected:
    QString generateCaptchaText(int length = 5);
    QPixmap drawCaptchaImage(const QString& text, QSize size = QSize(150, 50));
    void refreshCaptcha();

private: signals:
    void signalCaptchaSuccess();
};




#endif // CAPTCHAWIDGET_H
