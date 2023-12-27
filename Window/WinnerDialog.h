#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

class WinnerDialog : public QDialog
{
public:
    WinnerDialog(const QString& winnerMessage, const QColor& textColor, QWidget* parent = nullptr) : QDialog(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* winnerLabel = new QLabel(this);
        winnerLabel->setText(QString("<font color='%1'>%2</font>").arg(textColor.name()).arg(winnerMessage));
        layout->addWidget(winnerLabel);

        setLayout(layout);
    }
};
