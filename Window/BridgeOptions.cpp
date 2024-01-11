#include "BridgeOptions.h"
BridgeOptions::BridgeOptions(QWidget* parent)
    : QDialog{ parent }, selectedOptionIndex{ -1 }
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    messageLabelPlaceable = new QLabel(this);
    mainLayout->addWidget(messageLabelPlaceable);

    // Create a container for option labels
    optionsContainer = new QFrame(this);
    optionsLayout = new QVBoxLayout(optionsContainer);
    mainLayout->addWidget(optionsContainer);

    messageLabelRemovable = new QLabel(this);
    mainLayout->addWidget(messageLabelRemovable);

    optionsRemoveContainer = new QFrame(this);
    optionsRemoveLayout = new QVBoxLayout(optionsRemoveContainer);
    mainLayout->addWidget(optionsRemoveContainer);

    inputLineEdit = new QLineEdit(this);
    mainLayout->addWidget(inputLineEdit);

    addButton = new QPushButton("Add Bridge", this);
    connect(addButton, &QPushButton::clicked, this, &BridgeOptions::addBridge);
    mainLayout->addWidget(addButton);

    removeButton = new QPushButton("Remove Bridge", this);
    connect(removeButton, &QPushButton::clicked, this, &BridgeOptions::RemoveBridge);
    mainLayout->addWidget(removeButton);

    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &BridgeOptions::onCloseButtonClicked);
    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);
}

void BridgeOptions::setMessage(const QString& message1 , const QString& message2)
{
    messageLabelPlaceable->setText(message1);
    messageLabelRemovable->setText(message2);
}
void BridgeOptions::setBridgeInfo(const Bridge& bridge, const uint16_t& index, const bool & removable)
{
    QString info = QString::number(index) + ") " +
        QString::number(bridge.GetStartPillar().GetPosition().first) + " " +
        QString::number(bridge.GetStartPillar().GetPosition().second) + " " +
        colorToString(bridge.GetStartPillar().GetColor()) + " " +
        QString::number(bridge.GetEndPillar().GetPosition().first) + " " +
        QString::number(bridge.GetEndPillar().GetPosition().second) + " " +
        colorToString(bridge.GetEndPillar().GetColor());

    if (!removable)
    {
        QLabel* optionLabel = new QLabel(info, optionsContainer);
        optionsLayout->addWidget(optionLabel);
    }
    else
    {
        QLabel* optionLabel = new QLabel(info, optionsRemoveContainer);
        optionsRemoveLayout->addWidget(optionLabel);
    }
}
void BridgeOptions::setBridgeOptions(std::vector<Bridge> bridges)
{
    m_bridgeOptions = bridges;
}
std::vector<Bridge> BridgeOptions::getBridgeOptions()
{
    return m_bridgeOptions;
}
QString BridgeOptions::getUserInput()
{
    return inputLineEdit->text();
}
QString BridgeOptions::colorToString(Color color)
{
    switch (color)
    {
    case Color::RED:
        return "RED";
    case Color::BLACK:
        return "BLACK";
    case Color::NO_COLOR:
        return "NO_COLOR";
    default:
        return "";
    }
}

int BridgeOptions::getSelectedOptionIndex() const
{
    return selectedOptionIndex;
}

const std::vector<Bridge>& BridgeOptions::getRemovable() const
{
    return m_removable;
}

const std::vector<Bridge>& BridgeOptions::getPlaceable() const
{
    return m_placeable;
}
void BridgeOptions::setPlaceable(const std::vector<Bridge>& placeable)
{
    m_placeable = placeable;
}
void BridgeOptions::clearRemovableAndPlaceable()
{
    QLayoutItem* item;
    while ((item = optionsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    while ((item = optionsRemoveLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
void BridgeOptions::setRemovable(const std::vector<Bridge>& removable)
{
    m_removable = removable;
}
