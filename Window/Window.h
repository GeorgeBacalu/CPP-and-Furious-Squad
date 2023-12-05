#pragma once
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include<GameBoard.h>

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);
    ~Window();
    void setupUi();
    void paintEvent(QPaintEvent* event);
private:
    QGridLayout* layout;
    GameBoard* g;
};
