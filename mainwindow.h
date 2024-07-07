#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    void setupCustomTitleBar();
    QPoint m_dragPosition;
    bool m_dragging;
    bool m_resizing;
    QRect m_originalRect;

    bool isAtEdge(const QPoint &pos) const;
    bool isLeftEdge(const QPoint &pos) const;
    bool isRightEdge(const QPoint &pos) const;
    bool isTopEdge(const QPoint &pos) const;
    bool isBottomEdge(const QPoint &pos) const;

#ifdef Q_OS_WIN
    void setBlurEffect();
#endif
};

#endif // MAINWINDOW_H
