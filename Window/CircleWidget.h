#pragma once
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QPainter>
class CircleWidget : public QWidget
{
    Q_OBJECT
signals:
    void clicked();
private:
    QColor color;
public:
    explicit CircleWidget(QWidget* parent = nullptr) : QWidget(parent), color(Qt::white) {}

    void setColor(const QColor& newColor);

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event);

};

