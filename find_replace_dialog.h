//
// Created by iriso on 17.05.2026.
//

#ifndef NOTEPAD_FIND_REPLACE_DIALOG_H
#define NOTEPAD_FIND_REPLACE_DIALOG_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class find_replace_dialog;
}

QT_END_NAMESPACE

class find_replace_dialog : public QWidget {
    Q_OBJECT

public:
    explicit find_replace_dialog(QWidget *parent = nullptr);

    ~find_replace_dialog() override;

private:
    Ui::find_replace_dialog *ui;
};


#endif //NOTEPAD_FIND_REPLACE_DIALOG_H