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
    std::vector<Bridge>m_bridgeOptions;

public:
    BridgeOptions(QWidget* parent = nullptr);

    void setMessage(const QString& message);
    void setBridgeInfo(const Bridge& bridge, const uint16_t& index);
    void setBridgeOptions(std::vector<Bridge> bridges);
    std::vector<Bridge> getBridgeOptions();
    QString getUserInput();
    QString colorToString(Color color);

    int getSelectedOptionIndex() const;

public slots:
    void addBridge()
    {
        selectedOptionIndex = getUserInput().toInt();
        inputLineEdit->clear();
        emit addBridgeClicked();
    }
    void onCloseButtonClicked() // Slot for close button
    {
        emit closeButtonClicked();
    }

signals:
    void closeButtonClicked();
    void addBridgeClicked();
};
