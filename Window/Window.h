#pragma once
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QWidget>
#include <QPalette>
#include <QPainter>
#include <qpen.h>
#include<qmessagebox.h>
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
    void startEditing();
    void updateUiForPlaceableBridges(const std::vector<Bridge>& bridgeOptions,BridgeOptions* dialog);
    void advanceTurn();
    void chooseGameMode();
    void chooseGameType();
    void getPlayerNames();
    bool pillarsSpent(bool playerTurn);
    bool bridgesSpent(bool playerTurn);
private:
    QGridLayout* layout;
    GameBoard* g;
    BridgeOptions *dialog;
    QPushButton* editButton;
    QPushButton* stopEditButton;
    QPushButton* nextTurn;
    uint16_t turnNumber;
    std::string player1;
    std::string player2;
    bool winnerFound;
    bool editing;
    bool paintGameBoard;
    bool placedPillar;
    bool secondPlayerIsAI;
    bool normalGameMode;
};
