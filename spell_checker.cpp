#include "spell_checker.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include <algorithm>

SpellChecker::SpellChecker(QObject* parent)
    : QObject(parent)
{
}

bool SpellChecker::loadWordList(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString word = in.readLine().trimmed().toLower();
        if (!word.isEmpty())
            m_wordList.insert(word);
    }

    return !m_wordList.isEmpty();
}

bool SpellChecker::isCorrect(const QString& word) const
{
    if (word.isEmpty())
        return true;

    QString cleaned = word.toLower();
    cleaned.remove(QRegularExpression("[^a-z]"));

    if (cleaned.isEmpty())
        return true;

    return m_wordList.contains(cleaned);
}

QStringList SpellChecker::suggestions(const QString& word, int maxSuggestions) const
{
    QString cleaned = word.toLower();
    cleaned.remove(QRegularExpression("[^a-z]"));

    QList<QPair<int, QString>> candidates;

    for (const QString& dictWord : m_wordList) {
        if (qAbs(dictWord.length() - cleaned.length()) > 3)
            continue;

        int dist = editDistance(cleaned, dictWord);
        if (dist <= 2)
            candidates.append({ dist, dictWord });
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const QPair<int, QString>& a, const QPair<int, QString>& b) {
            return a.first < b.first;
        });

    QStringList result;
    for (int i = 0; i < qMin(maxSuggestions, (int)candidates.size()); ++i)
        result << candidates[i].second;

    return result;
}

int SpellChecker::editDistance(const QString& a, const QString& b) const
{
    int m = a.length();
    int n = b.length();

    QVector<QVector<int>> dp(m + 1, QVector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i)
        dp[i][0] = i;
    for (int j = 0; j <= n; ++j)
        dp[0][j] = j;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
    }

    return dp[m][n];
}