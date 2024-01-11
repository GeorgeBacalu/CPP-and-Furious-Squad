#include "Window.h"

Window::Window(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    layout = new QGridLayout;
    centralWidget->setLayout(layout);
    g = GameBoard::GetInstance();
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
    paintGameBoard = true;
    update();
    QPainter painter(this);
    g = GameBoard::GetInstance();
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (uint16_t i = 0; i < g->GetWidth(); i++)
    {
        for (uint16_t j = 0; j < g->GetHeight(); j++)
        {
            if ((i == 0 && (j == 0 || j == g->GetWidth() - 1)) ||
                (i == g->GetHeight() - 1 && (j == 0 || j == g->GetWidth() - 1)))
                    layout->addItem(new QSpacerItem(30, 30, QSizePolicy::Fixed, QSizePolicy::Fixed), i, j);
            else
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
    QPushButton* saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, this, &Window::onSaveClick);
    saveButton->setFixedSize(30 * g->GetWidth(), 30);
    layout->addWidget(saveButton, g->GetHeight(), 0, 1, g->GetWidth(), Qt::AlignHCenter);
    editButton = new QPushButton("Stop editing");
    connect(editButton, &QPushButton::clicked, this, &Window::stopEditing);
    editButton->setFixedSize(30 * g->GetWidth(), 30);
    editButton->setVisible(false);
    layout->addWidget(editButton, g->GetHeight() + 1, 0, 1, g->GetWidth(), Qt::AlignCenter);
    update();
}

void Window::loadGame()
{
    QPainter painter(this);
    clearLayout(layout);
    paintGameBoard = true;
    update();
    painter.setRenderHint(QPainter::Antialiasing, true);
    g = GameBoard::GetInstance();
    g->LoadGame();
    auto game{ g->GetMatrix() };
    for (uint16_t i = 0; i < game.size(); i++)
    {
        for (uint16_t j = 0; j < game.size(); j++)
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
    layout->addWidget(saveButton, g->GetHeight(), 0, 1, g->GetWidth(), Qt::AlignHCenter);
    editButton = new QPushButton("Stop editing");
    connect(editButton, &QPushButton::clicked, this, &Window::stopEditing);
    editButton->setFixedSize(30 * g->GetWidth(), 30);
    editButton->setVisible(false);
    layout->addWidget(editButton, g->GetHeight() + 1, 0, 1, g->GetWidth(), Qt::AlignCenter);
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
    for (const Bridge& br : b)
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
}

void Window::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.eraseRect(rect());
    drawBridges(painter);
    if (paintGameBoard)
    {
        CircleWidget* startCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(0,1)->widget());
        CircleWidget* endCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(0, g->GetWidth() - 2)->widget());

        QPoint start = startCircle->mapToParent(startCircle->rect().bottomLeft());
        QPoint end = endCircle->mapToParent(startCircle->rect().bottomRight());

        painter.setPen(QPen(Qt::red, 3, Qt::SolidLine));
        painter.drawLine(start,end);

        startCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(g->GetHeight() - 1, 1)->widget());
        endCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(g->GetHeight() - 1, g->GetWidth() - 2)->widget());

        start = startCircle->mapToParent(startCircle->rect().topLeft());
        end = endCircle->mapToParent(startCircle->rect().topRight());

        painter.drawLine(start, end);

        startCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(1,0)->widget());
        endCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(g->GetHeight() - 2,0)->widget());

        start = startCircle->mapToParent(startCircle->rect().topRight());
        end = endCircle->mapToParent(startCircle->rect().bottomRight());

        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        painter.drawLine(start, end);

        startCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(1, g->GetWidth()-1)->widget());
        endCircle = qobject_cast<CircleWidget*>(layout->itemAtPosition(g->GetHeight() - 2, g->GetWidth()-1)->widget());

        start = startCircle->mapToParent(startCircle->rect().topLeft());
        end = endCircle->mapToParent(startCircle->rect().bottomLeft());

        painter.drawLine(start, end);
    }
}
void Window::onCircleClick()
{
    CircleWidget* clickedCircle = qobject_cast<CircleWidget*>(sender());

    int row = clickedCircle->property("row").toInt();
    int col = clickedCircle->property("column").toInt();

    bool playerTurn = g->GetPlayerTurn();
    if (editing)
    {
        Color pillarColor = (!playerTurn) ? Color::RED : Color::BLACK;
        if(!g->IsFreeFoundation(row,col))
            if (g->GetMatrix()[row][col].value().GetColor() == pillarColor)
            {
                std::vector<Bridge> newBridges{ g->ProcessBridgesForNewPillar(g->GetMatrix()[row][col].value()) };
                PlaceBridgesFromOptions(newBridges, newBridges.size());
            }
            else
            {
                QMessageBox::warning(this, "Invalid Selection", "Please select your pillars");
            }
    }
    else
    {
        QColor newColor = (playerTurn) ? Qt::red : Qt::black;
        try
        {
            Pillar p;
            p.SetPosition(Position(row, col));
            p.SetColor((playerTurn) ? Color::RED : Color::BLACK);
            g->PlacePillarQT(row, col);
            clickedCircle->setColor(newColor);
            std::vector<Bridge> newBridges{ g->ProcessBridgesForNewPillar(p) };
            PlaceBridgesFromOptions(newBridges, newBridges.size());
        }
        catch (std::invalid_argument& exception)
        {
            std::cerr << exception.what() << "\n";
        }
        update();
    }
}
void Window::onSaveClick()
{
    g->SaveGame();
}

void Window::PlaceBridgesFromOptions(const std::vector<Bridge>& bridgeOptions, uint16_t numToPlace)
{
    if (bridgeOptions.size() > 0)
    {
        dialog = new BridgeOptions;
        dialog->setMessage("Choose a bridge to place:","Choose a bridge to remove");
        updateUiForPlaceableBridges(bridgeOptions, dialog);
        dialog->setBridgeOptions(bridgeOptions);
        //add the bridge from the placeable bridges and update the UI 
        connect(dialog, &BridgeOptions::addBridgeClicked, this, [&]() {
            int optionIndex = dialog->getSelectedOptionIndex();
            if (optionIndex >= 0 && optionIndex < dialog->getPlaceable().size()) {
                std::vector<Bridge> bridges = g->GetBridges();
                bridges.push_back(dialog->getPlaceable()[optionIndex]);
                g->SetBridges(bridges);
                updateUiForPlaceableBridges(dialog->getBridgeOptions(), dialog);
                checkWinner(!g->GetPlayerTurn());
                update();
            }
            });
        //delete the bridges that can be deleted , a bridge is considered START->END,END->START
        connect(dialog, &BridgeOptions::RemoveBridgeClicked, this, [&]() {
            int optionIndex = dialog->getSelectedOptionIndex();
            if (optionIndex >= 0 && optionIndex < dialog->getRemovable().size()) {
                Bridge removedBridge = dialog->getRemovable()[optionIndex];
                std::vector<Bridge> bridges = g->GetBridges();
                auto it = std::find(bridges.begin(), bridges.end(), removedBridge);
                if (it != bridges.end()) {
                    bridges.erase(it);
                }
                else
                {
                    it = std::find(bridges.begin(), bridges.end(), removedBridge.reverse());
                    if (it != bridges.end()) 
                        bridges.erase(it);
                }
                g->SetBridges(bridges);
                updateUiForPlaceableBridges(dialog->getBridgeOptions(), dialog);
                update();
            }
            });
        connect(dialog, &BridgeOptions::closeButtonClicked, this, [&]() 
            {
                dialog->reject();
                if (!editing && !winnerFound)
                {
                    QMessageBox msgBox;
                    msgBox.setText("Do you want to edit your pieces?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    QCoreApplication::processEvents();
                    int answer{ msgBox.exec() };
                    if (answer == QMessageBox::Yes)
                    {
                        editing = true;
                        editButton->setVisible(true);
                    }
                }
                update();
            });
        update();
        dialog->show();
    }

}
void Window::updateUiForPlaceableBridges(const std::vector<Bridge>& bridgeOptions, BridgeOptions* dialog)
{
    std::vector<Bridge> placeable, removable;
    std::vector<Bridge> bridges{ g->GetBridges() };
    for (const auto& bridge : bridgeOptions) {
        // Check if the bridge is in either direction
        if (std::find(bridges.begin(), bridges.end(), bridge) == bridges.end() &&
            std::find(bridges.begin(), bridges.end(), bridge.reverse()) == bridges.end()) {
            placeable.push_back(bridge);
        }
        else {
            removable.push_back(bridge);
        }
    }
    dialog->clearRemovableAndPlaceable();
    for (uint16_t i = 0; i < placeable.size(); ++i)
        dialog->setBridgeInfo(bridgeOptions[i], i, false);
    for (uint16_t i = 0; i < removable.size(); ++i)
        dialog->setBridgeInfo(bridgeOptions[i], i, true);

    dialog->setRemovable(removable);
    dialog->setPlaceable(placeable);
}
void Window::checkWinner(bool playerTurn)
{
    g->InitEndPillars();
    for (const auto& it : g->GetEndPillars())
    {
        g->UpdateAdjacencyList();
        if (it.GetColor() == Color::RED)
            g->SetRedPaths(GameBoard::BFS(it, g->GetAdjacencyList()));
        else
            g->SetBlackPaths(GameBoard::BFS(it, g->GetAdjacencyList()));
    }
    auto player = playerTurn ? Color::RED : Color::BLACK;
    if (g->CheckWin(player))
    {
        QColor winnerColor = (playerTurn) ? Qt::red : Qt::black;
        QString winnerMessage = (playerTurn) ? "Player RED Wins!" : "Player BLACK Wins!";
        WinnerDialog* winnerDialog = new WinnerDialog(winnerMessage, winnerColor, this);
        winnerDialog->show();
        QCoreApplication::processEvents();
        winnerFound = true;
        update();
    }
}
void Window::removePillar(uint16_t row, uint16_t column)
{
    CircleWidget* Circle = qobject_cast<CircleWidget*>(layout->itemAtPosition(row,column)->widget());
    Circle->setColor(Qt::white);
    g->RemovePillar(row, column);
}
void Window::stopEditing()
{
    editing = false;
    editButton->setVisible(false);
    update();
}
Window::~Window()
{}