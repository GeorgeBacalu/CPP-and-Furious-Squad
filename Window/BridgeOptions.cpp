#include "BridgeOptions.h"
BridgeOptions::BridgeOptions(QWidget* parent)
    : QDialog{ parent }, selectedOptionIndex{ -1 }
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    messageLabel = new QLabel(this);
    mainLayout->addWidget(messageLabel);

    // Create a container for option labels
    optionsContainer = new QFrame(this);
    optionsLayout = new QVBoxLayout(optionsContainer);
    mainLayout->addWidget(optionsContainer);

    inputLineEdit = new QLineEdit(this);
    mainLayout->addWidget(inputLineEdit);

    addButton = new QPushButton("Add Bridge", this);
    connect(addButton, &QPushButton::clicked, this, &BridgeOptions::addBridge);
    mainLayout->addWidget(addButton);

    closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &BridgeOptions::onCloseButtonClicked);
    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);
}

void BridgeOptions::setMessage(const QString& message)
{
    messageLabel->setText(message);
}
void BridgeOptions::setBridgeInfo(const Bridge& bridge, const uint16_t& index)
{
    QString info = QString::number(index) + ") " +
        QString::number(bridge.GetStartPillar().GetPosition().first) + " " +
        QString::number(bridge.GetStartPillar().GetPosition().second) + " " +
        colorToString(bridge.GetStartPillar().GetColor()) + " " +
        QString::number(bridge.GetEndPillar().GetPosition().first) + " " +
        QString::number(bridge.GetEndPillar().GetPosition().second) + " " +
        colorToString(bridge.GetEndPillar().GetColor());

    QLabel* optionLabel = new QLabel(info, optionsContainer);
    optionsLayout->addWidget(optionLabel);
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
