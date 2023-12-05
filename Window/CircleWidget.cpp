#include "CircleWidget.h"

void CircleWidget::setColor(const QColor& newColor)
{
	color = newColor;
	update();
}

void CircleWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(color);
    QRect ellipseRect(rect().center().x() - 10, rect().center().y() - 10, 20, 20);
    painter.drawEllipse(ellipseRect);
}

void CircleWidget::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit clicked();
}
