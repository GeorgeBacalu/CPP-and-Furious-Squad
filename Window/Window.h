#pragma once
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QWidget>
#include <QPalette>
#include <QPainter>
#include <qpen.h>
#include <QPushButton>
#include <GameBoard.h>
#include"BridgeOptions.h"
#include"CircleWidget.h"
#include"WinnerDialog.h"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);
    ~Window();
    void setupUi();
    void drawBridges(QPainter& painter);
    void onCircleClick();
    void onSaveClick();
    void paintEvent(QPaintEvent* event);
    void newGame();
    void loadGame();
    void clearLayout(QLayout * layout);
    void PlaceBridgesFromOptions(const std::vector<Bridge>& bridgeOptions, uint16_t numToPlace);
    void checkWinner(bool playerTurn);
    void removePillar(uint16_t row, uint16_t column);
    void stopEditing();
private:
    QGridLayout* layout;
    GameBoard* g;
    BridgeOptions *dialog;
    bool paintGameBoard;
    bool editing;
};
