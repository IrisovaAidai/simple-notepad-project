#include "spell_checker_highlighter.h"

#include <QRegularExpression>

SpellCheckerHighlighter::SpellCheckerHighlighter(SpellChecker* checker, QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , m_checker(checker)
{
    m_misspelledFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    m_misspelledFormat.setUnderlineColor(Qt::red);
}

void SpellCheckerHighlighter::highlightBlock(const QString& text)
{
    QRegularExpression wordPattern("\\b[a-zA-Z]+\\b");
    QRegularExpressionMatchIterator it = wordPattern.globalMatch(text);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString word = match.captured();

        if (!m_checker->isCorrect(word))
            setFormat(match.capturedStart(), match.capturedLength(), m_misspelledFormat);
    }
}