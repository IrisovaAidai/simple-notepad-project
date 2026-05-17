#pragma once

#include "spell_checker.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class SpellCheckerHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SpellCheckerHighlighter(SpellChecker* checker, QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    SpellChecker* m_checker;
    QTextCharFormat m_misspelledFormat;
};