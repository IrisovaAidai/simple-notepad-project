#pragma once

#include <QString>
#include <QTextCursor>

inline QString toUpperCase(const QString& text)
{
    return text.toUpper();
}

inline QString toLowerCase(const QString& text)
{
    return text.toLower();
}

inline QString toCapitalized(const QString& text)
{
    if (text.isEmpty())
        return text;

    QString result = text.toLower();
    result[0] = result[0].toUpper();
    return result;
}

inline QString toSentenceCase(const QString& text)
{
    if (text.isEmpty())
        return text;

    QString result = text.toLower();
    bool newSentence = true;

    for (int i = 0; i < result.size(); ++i) {
        if (newSentence && result[i].isLetter()) {
            result[i] = result[i].toUpper();
            newSentence = false;
        }
        if (result[i] == '.' || result[i] == '!' || result[i] == '?') {
            newSentence = true;
        }
    }

    return result;
}

inline QString toSwapCase(const QString& text)
{
    QString result = text;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i].isUpper())
            result[i] = result[i].toLower();
        else if (result[i].isLower())
            result[i] = result[i].toUpper();
    }
    return result;
}