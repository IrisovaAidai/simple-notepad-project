#pragma once

#include <QWidget>

// Forward declaration
class MainWindow;

// LineNumberArea is a helper for potential future line number display
// Currently used as a placeholder per project structure requirements
class LineNumberArea : public QWidget {
    Q_OBJECT

public:
    explicit LineNumberArea(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }
};