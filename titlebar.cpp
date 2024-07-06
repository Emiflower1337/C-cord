#include <QPushButton>

class TitleBarButton : public QPushButton {
private:
    const int buttonSize = 12;

protected:
    QImage icon;
    virtual void enterEvent(Event *) {
        // Draw the icon here
    }

    virtual void leaveEvent(Event *) {

    }

private:
    fromColor(QColor color) {
        this->setStyleSheet();
    }
};
