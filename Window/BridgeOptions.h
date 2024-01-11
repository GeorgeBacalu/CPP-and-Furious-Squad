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
    QLabel* messageLabelPlaceable;
    QLabel* messageLabelRemovable;
    QFrame* optionsContainer; 
    QVBoxLayout* optionsLayout;
    QFrame* optionsRemoveContainer;
    QVBoxLayout* optionsRemoveLayout;
    QLineEdit* inputLineEdit;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* closeButton;
    int selectedOptionIndex;
    std::vector<Bridge>m_bridgeOptions;
    std::vector<Bridge>m_removable;
    std::vector<Bridge>m_placeable;

public:
    BridgeOptions(QWidget* parent = nullptr);

    void setMessage(const QString& message1, const QString& message2);
    void setBridgeInfo(const Bridge& bridge, const uint16_t& index, const bool & removable);
    void setBridgeOptions(std::vector<Bridge> bridges);
    std::vector<Bridge> getBridgeOptions();
    const std::vector<Bridge>& getRemovable() const;
    const std::vector<Bridge>& getPlaceable() const;
    void setRemovable(const std::vector<Bridge>& removable);
    void setPlaceable(const std::vector<Bridge>& placeable);
    void clearRemovableAndPlaceable();
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
    void onCloseButtonClicked() 
    {
        emit closeButtonClicked();
    }
    void RemoveBridge()
    {
        selectedOptionIndex = getUserInput().toInt();
        inputLineEdit->clear();
        emit RemoveBridgeClicked();
    }

signals:
    void closeButtonClicked();
    void addBridgeClicked();
    void RemoveBridgeClicked();
};
