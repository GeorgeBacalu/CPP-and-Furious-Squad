#include "Window.h"
#include "GameBoard.h"
#include "Bridge.h"
#include "CircleWidget.h"
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QPainter>
#include <qpen.h>
#include <QPushButton>

Window::Window(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    layout = new QGridLayout;
    setupUi();
    centralWidget->setLayout(layout);
}
void Window::setupUi()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    g = GameBoard::getInstance();
    auto game{ g->getMatrix() };
    for (int i = 0; i < g->getSize(); i++)
    {
        for (int j = 0; j < g->getSize(); j++)
        {
            CircleWidget* circleWidget = new CircleWidget();
            circleWidget->setFixedSize(30, 30);
            circleWidget->setColor(Qt::white);
            circleWidget->setProperty("row", i);
            circleWidget->setProperty("column", j);
            connect(circleWidget, &CircleWidget::clicked, this, &Window::onCircleClick);
            layout->addWidget(circleWidget, i, j);
        }
    }
}

//void Window::loadGame()
//{
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    GameBoard* g = GameBoard::getInstance();
//    g->LoadGame();
//    auto game{ g->getMatrix() };
//    for (int i = 0; i < g->getSize(); i++)
//    {
//        for (int j = 0; j < g->getSize(); j++)
//        {   
//            CircleWidget circleWidget=
//            if (game[i][j].has_value())
//            {
//                if (game[i][j].value().GetColor() == Color::RED)
//                {
//                    circleWidget->setColor(Qt::red);
//                }
//                else
//                {
//                    circleWidget->setColor(Qt::black);
//                }
//            }
//            else
//            {
//                circleWidget->setColor(Qt::white);
//            }
//            layout->addWidget(circleWidget, i, j);
//        }
//    }
//}
void Window::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    GameBoard* g = GameBoard::getInstance();
    //g->LoadGame();

    for (Bridge b : g->getBridges())
    {
        Pillar pStart = b.GetStartPillar();
        Pillar pEnd = b.GetEndPillar();

        if (pStart.GetColor() == Color::RED)
            painter.setPen(QPen(Qt::red, 2, Qt::SolidLine));
        else
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));

        painter.drawLine(pStart.GetPosition().second * 30+30, pStart.GetPosition().first * 30+30 ,
            pEnd.GetPosition().second * 30 +30, pEnd.GetPosition().first * 30+30 );
    }
}

void Window::onCircleClick()
{
    CircleWidget* clickedCircle = qobject_cast<CircleWidget*>(sender());

    // Get the row and column of the clicked CircleWidget
    int row = clickedCircle->property("row").toInt();
    int col = clickedCircle->property("column").toInt();

    bool playerTurn = g->getPlayerTurn();
    QColor newColor = (playerTurn) ? Qt::red : Qt::black;
    g->switchPlayerTurn();
    clickedCircle->setColor(newColor);
}


Window::~Window()
{}

