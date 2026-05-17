#pragma once

#include "spell_checker.h"
#include "spell_checker_highlighter.h"
#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>

class SpellTextEdit : public QTextEdit {
    Q_OBJECT

public:
    explicit SpellTextEdit(SpellChecker* checker, QWidget* parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    SpellChecker* m_checker;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    // File
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

    // Edit
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();

    // Format - text case
    void toUpperCase();
    void toLowerCase();
    void toCapitalized();
    void toSentenceCase();
    void toSwapCase();

    // Format - rich text
    void toggleBold();
    void toggleItalic();
    void toggleUnderline();

    // Optional: Font dialog
    void openFontDialog();

    // Optional: Color picker
    void openColorPicker();

    // Find / Replace
    void openFindReplace();

    // Word frequency
    void openWordFrequency();

    // Tools
    void checkSpelling();

    // Status bar
    void updateStatusBar();
    void updateCursorPosition();

private:
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupEditor();
    void setupSpellChecker();

    void open_file(const QString& path);
    void save_file(const QString& path);

    bool maybeSave();
    void update_title();
    void applyTransform(std::function<QString(const QString&)> transform);

    // Widgets
    SpellTextEdit* m_editor = nullptr;

    // Status bar labels
    QLabel* m_wordCountLabel = nullptr;
    QLabel* m_lineCountLabel = nullptr;
    QLabel* m_cursorPosLabel = nullptr;

    // Spell checker
    SpellChecker* m_spellChecker = nullptr;
    SpellCheckerHighlighter* m_highlighter = nullptr;

    // State
    QString m_currentFile;
    bool m_modified = false;
};