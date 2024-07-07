#include <string>
#include <QPushButton>
#include <QPalette>
#include <QEvent>

class TitleBarButton : public QPushButton {
private:
    const int buttonSize = 12;
    QImage icon;

    // Private constructor
    TitleBarButton() {
        this->setFixedSize(buttonSize, buttonSize);
    }

    void updatePalette() {
        this->setPalette(palette);
        this->update();
    }

protected:    
    // MouseEvents
    virtual void enterEvent(QEvent *e) {
        // Draw the icon here

    }

    virtual void leaveEvent(QEvent *e) {

    }

public:
    QPalette *palette;

    void setPalette(QPalette *p) {
        this->palette = p;
        this->updatePalette();
    }

    // Factories
    static TitleBarButton fromPalette(QPalette p) {
        TitleBarButton r = new TitleBarButton();
        r->setPalette(p);

        return  r;
    }

    static TitleBarButton fromColor(QColor c) {
        TitleBarButton r = new TitleBarButton();
        QPalette p = new QPalette(c);
        r->setPalette(p);

        return r;
    }
};
