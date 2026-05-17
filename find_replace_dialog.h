#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class FindReplaceDialog;
}
QT_END_NAMESPACE

class FindReplaceDialog : public QDialog {
    Q_OBJECT

public:
    explicit FindReplaceDialog(QWidget* parent = nullptr);
    ~FindReplaceDialog() override;

    signals:
        void findNext(const QString& text);
    void replace(const QString& find, const QString& replace);
    void replaceAll(const QString& find, const QString& replace);

private:
    Ui::FindReplaceDialog* ui;
};