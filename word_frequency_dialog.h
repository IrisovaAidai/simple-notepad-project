#pragma once

#include <QDialog>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
    class WordFrequencyDialog;
}
QT_END_NAMESPACE

class WordFrequencyDialog : public QDialog {
    Q_OBJECT

public:
    explicit WordFrequencyDialog(const QMap<QString, int>& frequency, QWidget* parent = nullptr);
    ~WordFrequencyDialog() override;

private:
    Ui::WordFrequencyDialog* ui;
};