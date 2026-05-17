#pragma once

#include <QObject>
#include <QSet>
#include <QString>
#include <QStringList>

class SpellChecker : public QObject {
    Q_OBJECT

public:
    explicit SpellChecker(QObject* parent = nullptr);

    bool loadWordList(const QString& path);
    bool isCorrect(const QString& word) const;
    QStringList suggestions(const QString& word, int maxSuggestions = 5) const;

private:
    QSet<QString> m_wordList;

    int editDistance(const QString& a, const QString& b) const;
};