//
// Created by iriso on 17.05.2026.
//

#ifndef NOTEPAD_WORD_FREQUENCY_DIALOG_H
#define NOTEPAD_WORD_FREQUENCY_DIALOG_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class word_frequency_dialog;
}

QT_END_NAMESPACE

class word_frequency_dialog : public QWidget {
    Q_OBJECT

public:
    explicit word_frequency_dialog(QWidget *parent = nullptr);

    ~word_frequency_dialog() override;

private:
    Ui::word_frequency_dialog *ui;
};


#endif //NOTEPAD_WORD_FREQUENCY_DIALOG_H