#include "find_replace_dialog.h"
#include "ui_find_replace_dialog.h"

#include <QPushButton>

FindReplaceDialog::FindReplaceDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FindReplaceDialog)
{
    ui->setupUi(this);

    connect(ui->findNextButton, &QPushButton::clicked, this, [this]() {
        emit findNext(ui->findLineEdit->text());
    });

    connect(ui->replaceButton, &QPushButton::clicked, this, [this]() {
        emit replace(ui->findLineEdit->text(), ui->replaceLineEdit->text());
    });

    connect(ui->replaceAllButton, &QPushButton::clicked, this, [this]() {
        emit replaceAll(ui->findLineEdit->text(), ui->replaceLineEdit->text());
    });

    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

FindReplaceDialog::~FindReplaceDialog()
{
    delete ui;
}