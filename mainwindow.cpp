#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QPalette>
#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dragging(false)
    , m_resizing(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setupCustomTitleBar();
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

    QPushButton *minimizeButton = new QPushButton(titleBar);
    QPushButton *maximizeButton = new QPushButton(titleBar);
    QPushButton *closeButton = new QPushButton(titleBar);

    int buttonSize = 12;
    minimizeButton->setFixedSize(buttonSize, buttonSize);
    maximizeButton->setFixedSize(buttonSize, buttonSize);
    closeButton->setFixedSize(buttonSize, buttonSize);

    minimizeButton->setStyleSheet("background-color: #FFCC00; border-radius: 6px;");
    maximizeButton->setStyleSheet("background-color: #00CC00; border-radius: 6px;");
    closeButton->setStyleSheet("background-color: #FF3B30; border-radius: 6px;");

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
    painter.fillRect(rect(), QColor(45, 45, 45));
    QWidget::paintEvent(event);
}
