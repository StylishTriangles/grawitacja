#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>
#include "celestialbody.h"

QT_BEGIN_NAMESPACE
class QProgressBar;
class QPushButton;
QT_END_NAMESPACE
#ifndef M_G
#define M_G 6.67408313131
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323
#endif
#define MSEC_IN_SEC 1000
#define float_t long double

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;
    void collision(auto, auto);
    void updateButtons();
    void togglePause();
    template<typename T>
    inline constexpr T det(T x1, T y1, T x2, T y2) {return (x1*y2-x2*y1);}

private slots:
    void on_bouncyToggle_clicked();

    void on_pauseToggle_clicked();

    void on_gravityToggle_clicked();

    void on_borderToggle_clicked();

protected:
    Ui::MainWindow *ui;
    QProgressBar* ui_kappaBar;
    QPushButton* ui_bouncyToggle;
    QPushButton* ui_pauseToggle;
    QPushButton* ui_gravityToggle;
    QPushButton* ui_borderToggle;
    bool bouncyPhysics;
    bool simulationPaused;
    bool gravityPhysics;
    bool simulationBorder;
    int timerID;
    std::vector<celestialBody> cbodies;
    int deltaT; // in milliseconds

    const QString ui_bouncyToggleDisplayText[2] = {QString("Bouncy: OFF"),QString("Bouncy: ON")};
    const QString ui_pauseToggleDisplayText[2] = {QString("Pause: OFF"), QString("Pause: ON")};
    const QString ui_gravityToggleDisplayText[2] = {QString("Gravity: OFF"), QString("Gravity: ON")};
    const QString ui_borderToggleDisplayText[2] = {QString("Border: OFF"), QString("Border: ON")};
};

#endif // MAINWINDOW_H
