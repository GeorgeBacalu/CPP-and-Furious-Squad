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
    void drawBridges(QPainter& painter);
    void onCircleClick();
    void paintEvent(QPaintEvent* event);
    void newGame();
    void loadGame();
    void clearLayout(QLayout * layout);
private:
    QGridLayout* layout;
    GameBoard* g;
    bool needRepaint;
};
