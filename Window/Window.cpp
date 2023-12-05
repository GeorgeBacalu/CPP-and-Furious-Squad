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
            layout->addWidget(circleWidget, i, j);
        }
    }
}



Window::~Window()
{}

