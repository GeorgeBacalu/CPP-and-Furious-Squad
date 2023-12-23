#pragma once
#include <Bridge.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

class BridgeOptions : public QDialog
{
    Q_OBJECT
private:
    QLabel* messageLabel;
    QFrame* optionsContainer; 
    QVBoxLayout* optionsLayout;
    QLineEdit* inputLineEdit;
    QPushButton* addButton;
    QPushButton* closeButton;
    int selectedOptionIndex; 

public:
    BridgeOptions(QWidget* parent = nullptr)
        : QDialog(parent), selectedOptionIndex(-1)
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
        connect(closeButton, &QPushButton::clicked, this, &BridgeOptions::reject);
        mainLayout->addWidget(closeButton);

        setLayout(mainLayout);
    }

    void setMessage(const QString& message)
    {
        messageLabel->setText(message);
    }

    void setBridgeInfo(const Bridge& bridge, const uint16_t& index)
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

    QString getUserInput()
    {
        return inputLineEdit->text();
    }

    QString colorToString(Color color)
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

    int getSelectedOptionIndex() const
    {
        return selectedOptionIndex;
    }

public slots:
    void addBridge()
    {
        selectedOptionIndex = getUserInput().toInt();
        inputLineEdit->clear();
        emit addBridgeClicked();
    }

signals:
    void addBridgeClicked();
};
