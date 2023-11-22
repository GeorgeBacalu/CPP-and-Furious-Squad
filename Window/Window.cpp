#include "Window.h"
#include "GameBoard.h"
#include "Bridge.h"
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
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
    GameBoard* g = GameBoard::getInstance();
    g->LoadGame();
    auto game{ g->getMatrix() };
    for (int i = 0; i < g->getSize(); i++)
    {
        for (int j = 0; j < g->getSize(); j++)
        {
            QLabel* label = new QLabel();
            label->setAlignment(Qt::AlignCenter);
            label->setFrameStyle(QFrame::Box);
            label->setFixedSize(60, 60);
            if (game[i][j].has_value())
            {
                if (game[i][j].value().GetColor() == Color::RED)
                {
                    label->setStyleSheet("background-color: red");
                }
                else
                {
                    label->setStyleSheet("background-color: black");
                }
            }
            else
            {
                label->setStyleSheet("background-color: white");
            }
            layout->addWidget(label, i, j);
            label->repaint();
        }
    }

    setLayout(layout);
    setWindowTitle("Testedinnou");
}


Window::~Window()
{}