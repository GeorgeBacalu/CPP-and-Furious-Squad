#include "Window.h"

Window::Window(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    layout = new QGridLayout;
    centralWidget->setLayout(layout);
    setupUi();
}
void Window::setupUi()
{
    QPushButton* newGameButton = new QPushButton("New Game", this);
    connect(newGameButton, &QPushButton::clicked, this, &Window::newGame);
    layout->addWidget(newGameButton, 0, 0);
    QPushButton* loadGameButton = new QPushButton("Load Game", this);
    connect(loadGameButton, &QPushButton::clicked, this, &Window::loadGame);
    layout->addWidget(loadGameButton, 1, 0);

}
void Window::newGame()
{
    clearLayout(layout);
    update();
    QPainter painter(this);
    g = GameBoard::GetInstance();
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < g->GetWidth(); i++)
    {
        for (int j = 0; j < g->GetWidth(); j++)
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
    QPushButton* saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, this, &Window::onSaveClick);
    saveButton->setFixedSize(30 * g->GetWidth(), 30);
    layout->addWidget(saveButton, g->GetWidth(), 0, 1, g->GetWidth(), Qt::AlignHCenter);
}

void Window::loadGame()
{
    QPainter painter(this);
    clearLayout(layout);
    update();
    painter.setRenderHint(QPainter::Antialiasing, true);
    g = GameBoard::GetInstance();
    g->LoadGame();
    auto game{ g->GetMatrix() };
    for (int i = 0; i < game.size(); i++)
    {
        for (int j = 0; j < game.size(); j++)
        {   
            CircleWidget* circleWidget = new CircleWidget();
            circleWidget->setFixedSize(30, 30);
            circleWidget->setProperty("row", i);
            circleWidget->setProperty("column", j);
            connect(circleWidget, &CircleWidget::clicked, this, &Window::onCircleClick);
            if (game[i][j].has_value())
            {
                if (game[i][j].value().GetColor() == Color::RED)
                {
                    circleWidget->setColor(Qt::red);
                }
                else
                {
                    circleWidget->setColor(Qt::black);
                }
            }
            else
            {
                circleWidget->setColor(Qt::white);
            }
            layout->addWidget(circleWidget, i, j);
        }
    }
    QPushButton* saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, this, &Window::onSaveClick);
    saveButton->setFixedSize(30 * g->GetWidth(), 30);
    layout->addWidget(saveButton, g->GetWidth(), 0, 1, g->GetWidth(), Qt::AlignHCenter);
    needRepaint = true;
    update();
}

void Window::clearLayout(QLayout* layout)
{
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
void Window::drawBridges(QPainter& painter)
{
    std::vector<Bridge> b = g->GetBridges();
    for (Bridge br : b)
    {
        Pillar pStart = br.GetStartPillar();
        Pillar pEnd = br.GetEndPillar();

        if (pStart.GetColor() == Color::RED)
            painter.setPen(QPen(Qt::red, 2, Qt::SolidLine));
        else
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
        CircleWidget* startCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(pStart.GetPosition().first, pStart.GetPosition().second)->widget());
        CircleWidget* endCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(pEnd.GetPosition().first, pEnd.GetPosition().second)->widget());
        QPoint startCenter = startCircle->mapToParent(startCircle->rect().center());
        QPoint endCenter = endCircle->mapToParent(endCircle->rect().center());

        painter.drawLine(startCenter, endCenter);
    }
    needRepaint = false;
}

void Window::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    if (needRepaint)
    {
        QPainter painter(this);
        painter.eraseRect(rect());
        drawBridges(painter);
    }
}
void Window::onCircleClick()
{
    CircleWidget* clickedCircle = qobject_cast<CircleWidget*>(sender());

    int row = clickedCircle->property("row").toInt();
    int col = clickedCircle->property("column").toInt();

    bool playerTurn = g->GetPlayerTurn();

    QColor newColor = (playerTurn) ? Qt::red : Qt::black;
    try
    {
        Pillar p;
        p.SetPosition(Position(row, col));
        p.SetColor((playerTurn) ? Color::RED : Color::BLACK);
        g->PlacePillarQT(row, col);
        clickedCircle->setColor(newColor);
        needRepaint = true;
        repaint();
        std::vector<Bridge> newBridges{ g->ProcessNextMoveQT(p) };
        std::vector<Bridge> chosenBridges{ PlaceBridgesFromOptions(newBridges, newBridges.size()) };
        std::vector<Bridge> bridges = g->GetBridges();
        bridges.insert(bridges.end(), chosenBridges.begin(), chosenBridges.end());
        g->SetBridges(bridges);
        g->InitEndPillars();
        for (auto it : g->GetEndPillars())
            g->BFS(it);
        QCoreApplication::processEvents();
    }
    catch (std::invalid_argument& exception)
    {
        std::cerr << exception.what() << "\n";
    }
    needRepaint = true;
    update();
    auto player = playerTurn ? Color::RED : Color::BLACK;
    if (g->CheckWin(player))
    {
        QColor winnerColor = (playerTurn) ? Qt::red : Qt::black;
        QString winnerMessage = (playerTurn) ? "Player RED Wins!" : "Player BLACK Wins!";
        WinnerDialog* winnerDialog = new WinnerDialog(winnerMessage, winnerColor, this);
        winnerDialog->show();
        QCoreApplication::processEvents();
        needRepaint = true;
        update();
    }
    needRepaint = true;
    update();
}
void Window::onSaveClick()
{
    g->SaveGame();
}

std::vector<Bridge> Window::PlaceBridgesFromOptions(const std::vector<Bridge>& bridgeOptions, uint16_t numToPlace)
{
    std::vector<Bridge> chosenBridges;
    if (bridgeOptions.size() > 0)
    {
        QCoreApplication::processEvents();
        BridgeOptions dialog;
        dialog.setMessage("Choose a bridge to place:");
        for (uint16_t i = 0; i < numToPlace; ++i)
        {
             dialog.setBridgeInfo(bridgeOptions[i],i);
        }
       connect(&dialog, &BridgeOptions::addBridgeClicked, this, [&]() {
            int optionIndex = dialog.getSelectedOptionIndex();
            if (optionIndex >= 0 && optionIndex < bridgeOptions.size()) {
                chosenBridges.push_back(bridgeOptions[optionIndex]);
                needRepaint = true;
                update();
            }
        });

            dialog.exec();
            needRepaint = true;
            update();
    }
    return chosenBridges;
}
Window::~Window()
{}