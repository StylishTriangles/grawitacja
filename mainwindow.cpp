#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TIMER_INTERVAL 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bouncyPhysics(false),
    simulationPaused(false),
    gravityPhysics(true),
    simulationBorder(false),
    deltaT(TIMER_INTERVAL*3)
{
    ui->setupUi(this);
    ui_bouncyToggle = findChild<QPushButton*>("bouncyToggle");
    ui_pauseToggle = findChild<QPushButton*>("pauseToggle");
    ui_gravityToggle = findChild<QPushButton*>("gravityToggle");
    ui_borderToggle = findChild<QPushButton*>("borderToggle");
    this->updateButtons();
    // set black background
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    timerID = startTimer(TIMER_INTERVAL);

    //testing
    long double alfa = 0;
    long double v = 70;
    long double promien = 200;
    long double srodekX = 640;
    long double srodekY = 360;
    int NUMBER_OF_ORBS = 20;
    celestialBody cb;
    for (int i = 1; i <= NUMBER_OF_ORBS; i++)
    {

        cb.makePlanet(srodekX+cosl(alfa)*promien,srodekY+sinl(alfa)*promien);
        cb.setVelocity(sinl(alfa)*v,-cosl(alfa)*v);
        cbodies.push_back(cb);
        alfa+=2*M_PI/NUMBER_OF_ORBS;
    }
    alfa = 0;
    v = -30;
    promien = 100;
    srodekX = 640;
    srodekY = 360;
    NUMBER_OF_ORBS = 10;
    for (int i = 1; i <= NUMBER_OF_ORBS; i++)
    {
        cb.makePlanet(srodekX+cosl(alfa)*promien,srodekY+sinl(alfa)*promien);
        cb.setVelocity(sinl(alfa)*v,-cosl(alfa)*v);
        cb.setBrushColor(QColor(Qt::red));
        cb.setPenColor(QColor(200,100,100));
        cbodies.push_back(cb);
        alfa+=2*M_PI/NUMBER_OF_ORBS;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    for(celestialBody const& x:cbodies)
    {
        QPoint pos;
        painter.setPen(x.getPenColor());
        painter.setBrush(x.getBrushColor());
        pos.setX((int)x.getX());
        pos.setY((int)x.getY());
        painter.drawEllipse(pos,x.getWidth(),x.getHeight());
    }
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if (cbodies.empty())
    {
        this->update();
        return;
    }
    long double aX = 0.0;
    long double aY = 0.0;
    long double a = 0.0;
    long double F = 0.0;
    long double sinCB = 0.0;
    long double cosCB = 0.0;
    long double dist_2 = 0.0;

    std::pair<long double, long double>* deltaV = new std::pair<long double, long double>[cbodies.size()];
    // F = G*m1*m2/r^2
    // a = G*m2/r^2
    // ax = a.
    if (gravityPhysics)
    {
        for (unsigned i = 0; i < cbodies.size(); i++)
        {
            for (unsigned j = i+1; j < cbodies.size(); j++)
            {
                dist_2 = cbodies[i].distance_2(cbodies[j]);
                F=(cbodies[i].getMass()*cbodies[j].getMass()/dist_2)*M_G;
                cosCB = (cbodies[i].getX()-cbodies[j].getX())/sqrtl(dist_2);
                //QDebug(cosCB);
                sinCB = (cbodies[i].getY()-cbodies[j].getY())/sqrtl(dist_2);
                a=-F/cbodies[i].getMass();
                aX = a*cosCB;
                aY = a*sinCB;
                //cbodies[i].changeVelocity(aX*deltaT/MSEC_IN_SEC,aY*deltaT/MSEC_IN_SEC);
                deltaV[i].first += aX*deltaT/MSEC_IN_SEC;
                deltaV[i].second += aY*deltaT/MSEC_IN_SEC;

                //cbodies[i].move(deltaT);

                a=F/cbodies[j].getMass();
                aX = a*cosCB;
                aY = a*sinCB;
                //cbodies[j].changeVelocity(aX*deltaT/MSEC_IN_SEC,aY*deltaT/MSEC_IN_SEC);
                deltaV[j].first += aX*deltaT/MSEC_IN_SEC;
                deltaV[j].second += aY*deltaT/MSEC_IN_SEC;
                //cbodies[j].move(deltaT);
            }
        }
        //    for (unsigned i = 0; i < cbodies.size(); i++)
        //    {
        //        for (int j = 0; j < cbodies.size(); j++)
        //        {
        //            if (i==j)
        //                continue;
        //            dist_2 = cbodies[i].distance_2(cbodies[j]);
        //            cosCB = (cbodies[j].getX()-cbodies[i].getX())/sqrt(dist_2);
        //            sinCB = (cbodies[j].getY()-cbodies[i].getY())/sqrt(dist_2);
        //            a = M_G* (cbodies[j].getMass()/dist_2);
        //            aX = a * cosCB;
        //            aY = a * sinCB;
        //            cbodies[i].changeVelocity(aX*deltaT/MSEC_IN_SEC,aY*deltaT/MSEC_IN_SEC);
        //            cbodies[i].move(deltaT);
        //        }
        //    }
    }
    // odbicia miedzy obiektami
    if (bouncyPhysics)
    {
        for (unsigned i = 0; i < cbodies.size(); i++)
        {
            for (unsigned j = 0; j < cbodies.size(); j++)
            {
                if (i == j)
                    continue;
                if (cbodies[i].distance_2(cbodies[j]) > (cbodies[i].getRadius() + cbodies[j].getRadius())*(cbodies[i].getRadius() + cbodies[j].getRadius()))
                    continue;
                // niech cialo a porusza sie wzgledem ciala b, a cialo b bedzie nieruchome
                long double deltaPosX = cbodies[i].getX() - cbodies[j].getX();
                long double deltaPosY = cbodies[i].getY() - cbodies[j].getY();
                long double collisionAngle = atan2l(deltaPosX,deltaPosY);

                if (((deltaPosX < 0) xor (cbodies[i].getVx() < 0)) or ((deltaPosY < 0) xor (cbodies[i].getVy() < 0))) // jesli a zbliza sie do b
                {
                    long double deltaVx1, deltaVy1, deltaVx2, deltaVy2; // koncowe roznice szybkosci
                    long double velocityAngle = atan2l(cbodies[i].getVx(),cbodies[i].getVy());
                    long double alpha = velocityAngle-collisionAngle;
                    long double perpendicularAngle = collisionAngle + ((det(deltaPosX,deltaPosY,cbodies[i].getVx(),cbodies[i].getVy())<0)? (-M_PI/2) : (M_PI/2)); // perpendicular to collision angle
                    long double momentum = sqrtl(cbodies[i].getVx()*cbodies[i].getVx()+cbodies[i].getVy()*cbodies[i].getVy()); // not mult by mass yet
                    long double momentumX = cbodies[i].getVx()*cbodies[i].getMass();
                    long double momentumY = cbodies[i].getVy()*cbodies[i].getMass();
                    long double a = momentum * cosl(alpha);
                    deltaVx1 = a * cosl(perpendicularAngle);
                    deltaVy1 = a * sinl(perpendicularAngle);
                    momentum *= cbodies[i].getMass(); // now it's legit

                    momentumX -= deltaVx1 * cbodies[i].getMass();
                    momentumY -= deltaVy1 * cbodies[i].getMass();
                    deltaVx2 = momentumX/cbodies[j].getMass();
                    deltaVy2 = momentumY/cbodies[j].getMass();

                    deltaV[i].first += deltaVx1-cbodies[i].getVx();
                    deltaV[i].second += deltaVy1-cbodies[i].getVy();
                    deltaV[j].first += deltaVx2;
                    deltaV[j].second += deltaVy2;
                }
//                if (((deltaPosX > 0) xor (cbodies[j].getVx() < 0)) or ((deltaPosY > 0) xor (cbodies[j].getVy() < 0)))
//                {
//                    long double deltaVx1, deltaVy1, deltaVx2, deltaVy2; // koncowe roznice szybkosci
//                    long double velocityAngle = atan2l(cbodies[i].getVx(),cbodies[i].getVy());
//                    long double alpha = velocityAngle-collisionAngle;
//                    long double momentum = sqrtl(cbodies[i].getVx()*cbodies[i].getVx()+cbodies[i].getVy()*cbodies[i].getVy()); // not mult by mass yet
//                    long double momentumX = cbodies[i].getVx()*cbodies[i].getMass();
//                    long double momentumY = cbodies[i].getVy()*cbodies[i].getMass();
//                    long double a = momentum * cosl(alpha);
//                    deltaVx1 = a * cosl(velocityAngle);
//                    deltaVy1 = a * sinl(velocityAngle);
//                    momentum *= cbodies[i].getMass(); // now it's legit

//                    momentumX -= deltaVx1 * cbodies[i].getMass();
//                    momentumY -= deltaVy1 * cbodies[i].getMass();
//                    deltaVx2 = momentumX/cbodies[j].getMass();
//                    deltaVy2 = momentumY/cbodies[j].getMass();

//                    deltaV[i].first += deltaVx1;
//                    deltaV[i].second += deltaVy1;
//                    deltaV[j].first += deltaVx2;
//                    deltaV[j].second += deltaVy2;
//                }
            }
        }
    }
    // dodaj zmiany predkosci
    for(unsigned i = 0; i < cbodies.size(); i++)
        cbodies[i].changeVelocity(deltaV[i].first,deltaV[i].second);
    // sprawdz odbicia od krawedzi ekranu
    if (simulationBorder)
    {
        for (celestialBody &cb:cbodies)
        {
            if      (cb.getWidth()+(int)cb.getX() >= this->width()) // right
                cb.setVx(-cb.getVx());
            else if ((int)cb.getX()-cb.getWidth() <= 0) // left
                cb.setVx(-cb.getVx());
            if      (cb.getHeight()+(int)cb.getY() >= this->height()) // bottom
                cb.setVy(-cb.getVy());
            else if ((int)cb.getY()-cb.getHeight() <= 0) // top
                cb.setVy(-cb.getVy());
        }
    }
    // przesun obiekty
    for(celestialBody &x:cbodies)
        x.move(deltaT);
    delete [] deltaV;
    this->update();
}

void MainWindow::collision(auto cba, auto cbb)
{
    if (cba->distance_2(*cbb) > (cba->getRadius() + cbb->getRadius())*(cba->getRadius() + cbb->getRadius()))
        return;
    // niech cialo a porusza sie wzgledem ciala b, a cialo b bedzie nieruchome
    long double deltaVx1, deltaVy1, deltaVx2, deltaVy2; // koncowe roznice szybkosci
    long double deltaPosX = cba->getX() - cbb->getX();
    long double deltaPosY = cba->getY() - cbb->getY();
    long double collisionAngle = atan2l(deltaPosX,deltaPosY);
    long double velocityAngle = atan2l(cba->getVx(),cba->getVy());
    if (collisionAngle < 0) collisionAngle += 2.0*M_PI;
    if (velocityAngle < 0) velocityAngle += 2.0*M_PI;

    if (((deltaPosX < 0) xor (cba->getVx() < 0)) or ((deltaPosY < 0) xor (cba->getVy() < 0))) // jesli a zbliza sie do b
    {

    }
}

void MainWindow::updateButtons()
{
    ui_bouncyToggle->setText(ui_bouncyToggleDisplayText[bouncyPhysics]);
    ui_pauseToggle->setText(ui_pauseToggleDisplayText[simulationPaused]);
    ui_gravityToggle->setText(ui_gravityToggleDisplayText[gravityPhysics]);
    ui_borderToggle->setText(ui_borderToggleDisplayText[simulationBorder]);
}

void MainWindow::togglePause()
{
    simulationPaused = !simulationPaused;
    if (simulationPaused)
        killTimer(timerID);
    else
        timerID = startTimer(TIMER_INTERVAL);
}

void MainWindow::on_bouncyToggle_clicked()
{
    bouncyPhysics = !bouncyPhysics;
    ui_bouncyToggle->setText(ui_bouncyToggleDisplayText[bouncyPhysics]);
}


void MainWindow::on_pauseToggle_clicked()
{
    this->togglePause();
    ui_pauseToggle->setText(ui_pauseToggleDisplayText[simulationPaused]);
}

void MainWindow::on_gravityToggle_clicked()
{
    gravityPhysics = !gravityPhysics;
    ui_gravityToggle->setText(ui_gravityToggleDisplayText[gravityPhysics]);
}

void MainWindow::on_borderToggle_clicked()
{
    simulationBorder = !simulationBorder;
    ui_borderToggle->setText(ui_borderToggleDisplayText[simulationBorder]);
}
