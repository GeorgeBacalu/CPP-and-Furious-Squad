#include "Window.h"

Window::Window(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    turnNumber = 0;
    layout = new QGridLayout;
    centralWidget->setLayout(layout);
    g = GameBoard::GetInstance();
    chooseGameMode();
}
void Window::setupUi()
{
    clearLayout(layout);
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

    stopEditButton = new QPushButton("Stop editing");
    connect(stopEditButton, &QPushButton::clicked, this, &Window::stopEditing);
    stopEditButton->setFixedSize(30 * g->GetWidth(), 30);
    stopEditButton->setVisible(false);
    layout->addWidget(stopEditButton, g->GetHeight() + 1, 0, 1, g->GetWidth(), Qt::AlignCenter);

    editButton = new QPushButton("Start editing");
    connect(editButton, &QPushButton::clicked, this, &Window::startEditing);
    editButton->setFixedSize(30 * g->GetWidth(), 30);
    editButton->setVisible(true);
    layout->addWidget(editButton, g->GetHeight() + 1, 0, 1, g->GetWidth(), Qt::AlignCenter);

    nextTurn = new QPushButton("Advance Turn");
    connect(nextTurn, &QPushButton::clicked, this, &Window::advanceTurn);
    nextTurn->setFixedSize(30 * g->GetWidth(), 30);
    nextTurn->setVisible(true);
    layout->addWidget(nextTurn, g->GetHeight() + 2, 0, 1, g->GetWidth(), Qt::AlignCenter);
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

    stopEditButton = new QPushButton("Stop editing");
    connect(stopEditButton, &QPushButton::clicked, this, &Window::stopEditing);
    stopEditButton->setFixedSize(30 * g->GetWidth(), 30);
    stopEditButton->setVisible(false);
    layout->addWidget(stopEditButton, g->GetHeight() + 1, 0, 1, g->GetWidth(), Qt::AlignCenter);

    editButton = new QPushButton("Start editing");
    connect(editButton, &QPushButton::clicked, this, &Window::startEditing);
    editButton->setFixedSize(30 * g->GetWidth(), 30);
    editButton->setVisible(true);
    layout->addWidget(editButton, g->GetHeight() + 1, 0, 1, g->GetWidth(), Qt::AlignCenter);

    nextTurn = new QPushButton("Advance Turn");
    connect(nextTurn, &QPushButton::clicked, this, &Window::advanceTurn);
    nextTurn->setFixedSize(30 * g->GetWidth(), 30);
    nextTurn->setVisible(true);
    layout->addWidget(nextTurn, g->GetHeight() + 2, 0, 1, g->GetWidth(), Qt::AlignCenter);
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

        QColor turnColor = (g->GetPlayerTurn()) ? Qt::red : Qt::black;
        QString turnText = QString::fromStdString((g->GetPlayerTurn()) ? (player1+"s Turn") : (player2+"s Turn"));
        painter.setPen(QPen(turnColor));
        painter.setFont(QFont("Arial", 10));
        painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, turnText);
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
        Color pillarColor = (playerTurn) ? Color::RED : Color::BLACK;
        if(!g->IsFreeFoundation(row,col))
            if (g->GetMatrix()[row][col].value().GetColor() == pillarColor)
            {
                std::vector<Bridge> newBridges{ g->ProcessBridgesForNewPillar(g->GetMatrix()[row][col].value()) };
                PlaceBridgesFromOptions(newBridges, newBridges.size());
            }
            else
            {
                if (!g->GetMatrix()[row][col].has_value() || g->GetMatrix()[row][col].value().GetColor() != pillarColor)
                    QMessageBox::warning(this, "Invalid Selection", "Please select your pillars");
            }
    }
    else
    {
        if (placedPillar)
        {
            QMessageBox::warning(this, "Invalid Action", "You already placed a pillar");
            return;
        }
        QColor newColor = (playerTurn) ? Qt::red : Qt::black;
        if (g->GetPlayerTurn())
        {
            if (g->getRedPillarsSpent())
            {
                QMessageBox::warning(this, "Invalid Action", "You have no pillars left");
                return;
            }
        }
        else
            if (g->getBlackPillarsSpent())
            {
                QMessageBox::warning(this, "Invalid Action", "You have no pillars left");
                return;
            }
        try
        {
            Pillar p;
            p.SetPosition(Position(row, col));
            p.SetColor((playerTurn) ? Color::RED : Color::BLACK);
            g->PlacePillarQT(row, col);
            g->checkPieces();
            clickedCircle->setColor(newColor);
            std::vector<Bridge> newBridges{ g->ProcessBridgesForNewPillar(p) };
            PlaceBridgesFromOptions(newBridges, newBridges.size());
            placedPillar = true;
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
        dialog->setMessage("Choose a bridge to place:", "Choose a bridge to remove");
        updateUiForPlaceableBridges(bridgeOptions, dialog);
        dialog->setBridgeOptions(bridgeOptions);
        //add the bridge from the placeable bridges and update the UI 
        connect(dialog, &BridgeOptions::addBridgeClicked, this, [&]() {
            if (g->GetPlayerTurn())
            {
                if (g->getRedBridgesSpent())
                {
                    QMessageBox::warning(this, "Invalid Action", "You have no bridges left");
                    return;
                }
            }
            else
                if (g->getBlackBridgesSpent())
                {
                    QMessageBox::warning(this, "Invalid Action", "You have no bridges left");
                    return;
                }
            int optionIndex = dialog->getSelectedOptionIndex();
            if (optionIndex >= 0 && optionIndex < dialog->getPlaceable().size()) {
                std::vector<Bridge> bridges = g->GetBridges();
                bridges.push_back(dialog->getPlaceable()[optionIndex]);
                g->SetBridges(bridges);
                if (g->GetPlayerTurn())
                    g->setRedBridgesCount(g->getRedBridgesCount()+1);
                else
                    g->setBlackBridgesCount(g->getBlackBridgesCount() + 1);
                updateUiForPlaceableBridges(dialog->getBridgeOptions(), dialog);
                g->checkPieces();
                checkWinner(g->GetPlayerTurn());
                update();
            }
            });
        //delete  the bridge from the removable bridges . A bridge is considered START->END,END->START
        connect(dialog, &BridgeOptions::RemoveBridgeClicked, this, [&]() {
            int optionIndex = dialog->getSelectedOptionIndex();
            if (optionIndex >= 0 && optionIndex < dialog->getRemovable().size()) {
                Bridge removedBridge = dialog->getRemovable()[optionIndex];
                g->RemoveBridge(removedBridge);
                updateUiForPlaceableBridges(dialog->getBridgeOptions(), dialog);
                update();
            }
            });
        connect(dialog, &BridgeOptions::closeButtonClicked, this, [&]()
            {
                dialog->reject();
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
        dialog->setBridgeInfo(placeable[i], i, false);
    for (uint16_t i = 0; i < removable.size(); ++i)
        dialog->setBridgeInfo(removable[i], i, true);

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
    stopEditButton->setVisible(false);
    if(!winnerFound)
        checkWinner(g->GetPlayerTurn());
    editButton->setVisible(true);
    g->SwitchPlayerTurn();
    update();
}
void Window::startEditing()
{
    if (!placedPillar)
    {
        QMessageBox::warning(this, "Invalid Action", "Please place your pillar");
    }
    else
    {
        editing = true;
        editButton->setVisible(false);
        stopEditButton->setVisible(true);
    }
    update();
}
void Window::advanceTurn()
{
    if (editing)
    {
        stopEditing();
        placedPillar = false;
        return;
    }
    if (!placedPillar)
    {
        QMessageBox::warning(this, "Invalid Action", "Please place your pillar");
        return;
    }
    if (g->getRedPillarsSpent() && g->getBlackPillarsSpent())
    {
        QMessageBox msgBox;
        msgBox.setText("DRAW");
        return;
    }
    turnNumber++;
    g->SwitchPlayerTurn();
    update();
    if (turnNumber == 1 && !secondPlayerIsAI)
    {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(player2+" do you want to switch your color?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int answer = msgBox.exec();
        QCoreApplication::processEvents();
        if (answer == QMessageBox::Yes)
            std::swap(player1, player2);
    }
    update();
    placedPillar = false;
    
}
void Window::chooseGameMode()
{
    QLabel* label = new QLabel("Please select your GameMode", this);
    layout->addWidget(label, 0, 0, Qt::AlignCenter);

    QPushButton* GameButton = new QPushButton("Normal Mode", this);
    connect(GameButton, &QPushButton::clicked, [=]() {
        normalGameMode = true;
        chooseGameType();
        });
    layout->addWidget(GameButton, 1, 0, Qt::AlignCenter);
}
void Window::chooseGameType()
{
    clearLayout(layout);
    QLabel* label = new QLabel("Please select your oponent", this);
    layout->addWidget(label, 0, 0, Qt::AlignCenter);

    QPushButton* GameButton = new QPushButton("Player vs Player ", this);
    connect(GameButton, &QPushButton::clicked, this, &Window::getPlayerNames);
    layout->addWidget(GameButton, 1, 0, Qt::AlignCenter);
}
void Window::getPlayerNames()
{
    clearLayout(layout);
    QLabel* player1Label = new QLabel("Enter Player1's Name:", this);
    layout->addWidget(player1Label, 0, 0);

    QLineEdit* name1LineEdit = new QLineEdit(this);
    layout->addWidget(name1LineEdit, 0, 1);
    QLabel* player2Label = new QLabel("Enter Player2's Name:", this);
    layout->addWidget(player2Label, 1, 0);

    QLineEdit* name2LineEdit = new QLineEdit(this);
    layout->addWidget(name2LineEdit, 1, 1);
    if (secondPlayerIsAI)
    {
        player2Label->setVisible(false);
        name2LineEdit->setVisible(false);
    }
    QPushButton* getPlayerButton = new QPushButton("Next", this);
    connect(getPlayerButton, &QPushButton::clicked, this, [=]() {
        QString name = name1LineEdit->text();
        player1 = name.toStdString();
        if (!secondPlayerIsAI)
        {
            name = name2LineEdit->text();
            player2 = name.toStdString();
        }
        else
            player2 = "AI";
        setupUi();
        });
    if(!secondPlayerIsAI)
        layout->addWidget(getPlayerButton, 2, 0, Qt::AlignCenter);
    else
        layout->addWidget(getPlayerButton, 1, 0, Qt::AlignCenter);
}
Window::~Window()
{}