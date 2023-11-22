#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Window.h"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);
    ~Window();

private:
    Ui::WindowClass ui;
};
