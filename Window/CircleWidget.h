#pragma once
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QPainter>
class CircleWidget : public QWidget
{
private:
    QColor color;
public:
    explicit CircleWidget(QWidget* parent = nullptr) : QWidget(parent), color(Qt::white) {}

    void setColor(const QColor& newColor);

protected:
    void paintEvent(QPaintEvent* event) override;

};

