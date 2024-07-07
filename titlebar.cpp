#include "titlebar.h"
#include <QDebug>
#include <QPainter>

TitleBarButton::TitleBarButton() {
    qDebug() << "TitleBarButton constructor";
    this->setFixedSize(buttonSize, buttonSize);
    this->setFlat(true);
}

void TitleBarButton::updatePalette() {
    qDebug() << "updatePalette start";
    QPushButton::setPalette(palette);
    this->update();
    qDebug() << "updatePalette end";
}

void TitleBarButton::enterEvent(QEnterEvent *e) {
    qDebug() << "enterEvent";
    QPushButton::enterEvent(e);
}

void TitleBarButton::leaveEvent(QEvent *e) {
    qDebug() << "leaveEvent";
    QPushButton::leaveEvent(e);
}

void TitleBarButton::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(palette.window());
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, buttonSize, buttonSize);
    QPushButton::paintEvent(e);
}

void TitleBarButton::setPalette(const QPalette &p) {
    qDebug() << "setPalette start";
    palette = p;
    this->updatePalette();
    qDebug() << "setPalette end";
}

TitleBarButton* TitleBarButton::fromPalette(const QPalette &p) {
    qDebug() << "fromPalette";
    TitleBarButton *r = new TitleBarButton();
    r->setPalette(p);
    return r;
}

TitleBarButton* TitleBarButton::fromColor(const QColor &c) {
    qDebug() << "fromColor";
    TitleBarButton *r = new TitleBarButton();
    QPalette p;
    p.setColor(QPalette::Window, c);
    r->setPalette(p);
    return r;
}
