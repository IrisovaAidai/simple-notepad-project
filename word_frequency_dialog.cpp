#include "word_frequency_dialog.h"
#include "ui_word_frequency_dialog.h"

#include <QList>
#include <QPair>
#include <QTableWidgetItem>
#include <algorithm>

WordFrequencyDialog::WordFrequencyDialog(const QMap<QString, int>& frequency, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::WordFrequencyDialog)
{
    ui->setupUi(this);

    QList<QPair<QString, int>> sorted;
    for (auto it = frequency.constBegin(); it != frequency.constEnd(); ++it)
        sorted.append({ it.key(), it.value() });

    std::sort(sorted.begin(), sorted.end(),
        [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
            return a.second > b.second;
        });

    ui->frequencyTable->setRowCount(sorted.size());
    ui->frequencyTable->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < sorted.size(); ++i) {
        ui->frequencyTable->setItem(i, 0, new QTableWidgetItem(sorted[i].first));
        ui->frequencyTable->setItem(i, 1, new QTableWidgetItem(QString::number(sorted[i].second)));
    }

    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

WordFrequencyDialog::~WordFrequencyDialog()
{
    delete ui;
}