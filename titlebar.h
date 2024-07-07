#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QPushButton>
#include <QPalette>
#include <QEnterEvent>
#include <QColor>

class TitleBarButton : public QPushButton {
    Q_OBJECT

private:
    const int buttonSize = 12;
    QPalette palette;

    TitleBarButton();

    void updatePalette();

protected:
    void enterEvent(QEnterEvent *e) override;
    void leaveEvent(QEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

public:
    static TitleBarButton* fromPalette(const QPalette &p);
    static TitleBarButton* fromColor(const QColor &c);

    void setPalette(const QPalette &p);
};

#endif // TITLEBAR_H
