#pragma once

#include <QString>
#include <QStringList>

inline QString sortLines(const QString& text, Qt::SortOrder order = Qt::AscendingOrder)
{
    QStringList lines = text.split('\n');
    lines.sort(Qt::CaseInsensitive);

    if (order == Qt::DescendingOrder)
        std::reverse(lines.begin(), lines.end());

    return lines.join('\n');
}