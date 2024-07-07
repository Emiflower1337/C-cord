#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "titlebar.cpp"
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QPalette>
#include <QPainter>
#include <QMouseEvent>
#include <QOperatingSystemVersion>

//#ifdef Q_OS_WIN
//#include <dwmapi.h>
//#pragma comment(lib, "Dwmapi.lib")

//struct ACCENT_POLICY {
//    int nAccentState;
//    int nFlags;
//    int nColor;
//    int nAnimationId;
//};

//struct WINCOMPATTRDATA {
//    int nAttribute;
//    void* pData;
//    ULONG ulDataSize;
//};

//#define ACCENT_ENABLE_BLURBEHIND 3
//#define WCA_ACCENT_POLICY 19

//typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
//#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dragging(false)
    , m_resizing(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);

    setupCustomTitleBar();
    //setBlurEffect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCustomTitleBar()
{
    QWidget *titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    setPalette(darkPalette);

    TitleBarButton *minimizeButton = TitleBarButton::fromColor(QColor::fromRgb(0xFF, 0xCC, 0x00));
    minimizeButton->setParent(titleBar);
    TitleBarButton *maximizeButton = TitleBarButton::fromColor(QColor::fromRgb(0x00, 0xCC, 0x00));
    minimizeButton->setParent(titleBar);
    TitleBarButton *closeButton = TitleBarButton::fromColor(QColor::fromRgb(0xFF, 0x3b, 0x30));
    minimizeButton->setParent(titleBar);

    // int buttonSize = 12;
    // minimizeButton->setFixedSize(buttonSize, buttonSize);
    // maximizeButton->setFixedSize(buttonSize, buttonSize);
    // closeButton->setFixedSize(buttonSize, buttonSize);

    // minimizeButton->setStyleSheet("background-color: #FFCC00; border-radius: 6px;");
    // maximizeButton->setStyleSheet("background-color: #00CC00; border-radius: 6px;");
    // closeButton->setStyleSheet("background-color: #FF3B30; border-radius: 6px;");

    connect(minimizeButton, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    connect(maximizeButton, &QPushButton::clicked, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(closeButton, &QPushButton::clicked, this, &QMainWindow::close);

    QHBoxLayout *layout = new QHBoxLayout(titleBar);
    layout->addWidget(minimizeButton);
    layout->addWidget(maximizeButton);
    layout->addWidget(closeButton);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 5, 0);

    setMenuWidget(titleBar);
}

//#ifdef Q_OS_WIN
//void MainWindow::setBlurEffect()
//{
//    HWND hwnd = reinterpret_cast<HWND>(winId());

//    if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10) {
//        // For Windows 10 and later
//        const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
//        if (hModule) {
//            pSetWindowCompositionAttribute SetWindowCompositionAttribute = reinterpret_cast<pSetWindowCompositionAttribute>(GetProcAddress(hModule, "SetWindowCompositionAttribute"));

//            if (SetWindowCompositionAttribute) {
//                ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
//                WINCOMPATTRDATA data = { WCA_ACCENT_POLICY, &accent, sizeof(accent) };
//                SetWindowCompositionAttribute(hwnd, &data);
//            }
//            FreeLibrary(hModule);
//        }
//    } else {
//        // For Windows Vista to 8.1
//        DWM_BLURBEHIND bb = { 0 };
//        bb.dwFlags = DWM_BB_ENABLE;
//        bb.fEnable = true;
//        bb.hRgnBlur = nullptr;

//        DwmEnableBlurBehindWindow(hwnd, &bb);
//    }
//}
//#endif

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (event->pos().y() <= 30) {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
            m_dragging = true;
        } else if (isAtEdge(event->pos())) {
            m_dragPosition = event->globalPos();
            m_resizing = true;
            m_originalRect = frameGeometry();
        }
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    } else if (m_resizing && (event->buttons() & Qt::LeftButton)) {
        QRect newRect = m_originalRect;
        QPoint diff = event->globalPos() - m_dragPosition;

        if (isLeftEdge(m_dragPosition)) {
            newRect.setLeft(newRect.left() + diff.x());
        }
        if (isRightEdge(m_dragPosition)) {
            newRect.setRight(newRect.right() + diff.x());
        }
        if (isTopEdge(m_dragPosition)) {
            newRect.setTop(newRect.top() + diff.y());
        }
        if (isBottomEdge(m_dragPosition)) {
            newRect.setBottom(newRect.bottom() + diff.y());
        }

        setGeometry(newRect);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        m_resizing = false;
        event->accept();
    }
}

bool MainWindow::isAtEdge(const QPoint &pos) const
{
    return isLeftEdge(pos) || isRightEdge(pos) || isTopEdge(pos) || isBottomEdge(pos);
}

bool MainWindow::isLeftEdge(const QPoint &pos) const
{
    return pos.x() <= 5;
}

bool MainWindow::isRightEdge(const QPoint &pos) const
{
    return pos.x() >= width() - 5;
}

bool MainWindow::isTopEdge(const QPoint &pos) const
{
    return pos.y() <= 5;
}

bool MainWindow::isBottomEdge(const QPoint &pos) const
{
    return pos.y() >= height() - 5;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QColor backgroundColor = QColor(45, 45, 45 /*, 180*/);
    painter.fillRect(rect(), backgroundColor);
    QWidget::paintEvent(event);
}
