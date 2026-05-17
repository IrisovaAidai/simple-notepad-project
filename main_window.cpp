#include "main_window.h"

#include "notepad_exception.h"
#include "text_transform.h"
#include "sort.h"
#include "find_replace_dialog.h"
#include "word_frequency_dialog.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QColorDialog>
#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupEditor();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupSpellChecker();

    update_title();
    resize(900, 650);
}

void MainWindow::setupEditor()
{
    m_editor = new QTextEdit(this);
    m_editor->setAcceptRichText(true);
    setCentralWidget(m_editor);

    connect(m_editor, &QTextEdit::textChanged, this, [this]() {
        m_modified = true;
        updateStatusBar();
    });

    connect(m_editor, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateCursorPosition);
}

void MainWindow::setupMenuBar()
{
    // ── File ──────────────────────────────────
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New", this, &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction("Open...", this, &MainWindow::openFile, QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction("Save", this, &MainWindow::saveFile, QKeySequence::Save);
    fileMenu->addAction("Save As...", this, &MainWindow::saveFileAs, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close, QKeySequence::Quit);

    // ── Edit ──────────────────────────────────
    QMenu* editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Undo", this, &MainWindow::undo, QKeySequence::Undo);
    editMenu->addAction("Redo", this, &MainWindow::redo, QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction("Cut", this, &MainWindow::cut, QKeySequence::Cut);
    editMenu->addAction("Copy", this, &MainWindow::copy, QKeySequence::Copy);
    editMenu->addAction("Paste", this, &MainWindow::paste, QKeySequence::Paste);
    editMenu->addSeparator();
    editMenu->addAction("Select All", this, &MainWindow::selectAll, QKeySequence::SelectAll);

    // ── Format ────────────────────────────────
    QMenu* formatMenu = menuBar()->addMenu("Format");

    QAction* boldAction = formatMenu->addAction("Bold", this, &MainWindow::toggleBold, QKeySequence::Bold);
    boldAction->setCheckable(true);
    QAction* italicAction = formatMenu->addAction("Italic", this, &MainWindow::toggleItalic, QKeySequence::Italic);
    italicAction->setCheckable(true);
    QAction* underlineAction = formatMenu->addAction("Underline", this, &MainWindow::toggleUnderline, QKeySequence::Underline);
    underlineAction->setCheckable(true);

    formatMenu->addSeparator();
    formatMenu->addAction("Font...", this, &MainWindow::openFontDialog);
    formatMenu->addAction("Text Color...", this, &MainWindow::openColorPicker);
    formatMenu->addSeparator();

    QMenu* caseMenu = formatMenu->addMenu("Text Case");
    caseMenu->addAction("UPPERCASE", this, &MainWindow::toUpperCase);
    caseMenu->addAction("lowercase", this, &MainWindow::toLowerCase);
    caseMenu->addAction("Capitalize", this, &MainWindow::toCapitalized);
    caseMenu->addAction("Sentence case", this, &MainWindow::toSentenceCase);
    caseMenu->addAction("sWAP cASE", this, &MainWindow::toSwapCase);

    // ── Search ────────────────────────────────
    QMenu* searchMenu = menuBar()->addMenu("Search");
    searchMenu->addAction("Find / Replace...", this, &MainWindow::openFindReplace, QKeySequence::Find);

    // ── Tools ─────────────────────────────────
    QMenu* toolsMenu = menuBar()->addMenu("Tools");
    toolsMenu->addAction("Word Frequency...", this, &MainWindow::openWordFrequency);
    toolsMenu->addAction("Check Spelling...", this, &MainWindow::checkSpelling);
}

void MainWindow::setupToolBar()
{
    QToolBar* toolbar = addToolBar("Formatting");

    QAction* boldAct = toolbar->addAction("B", this, &MainWindow::toggleBold);
    boldAct->setCheckable(true);
    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    QAction* italicAct = toolbar->addAction("I", this, &MainWindow::toggleItalic);
    italicAct->setCheckable(true);
    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    QAction* underlineAct = toolbar->addAction("U", this, &MainWindow::toggleUnderline);
    underlineAct->setCheckable(true);
    QFont underlineFont = underlineAct->font();
    underlineFont.setUnderline(true);
    underlineAct->setFont(underlineFont);
}

void MainWindow::setupStatusBar()
{
    m_wordCountLabel = new QLabel("Words: 0", this);
    m_lineCountLabel = new QLabel("Lines: 1", this);
    m_cursorPosLabel = new QLabel("Ln 1, Col 1", this);

    statusBar()->addPermanentWidget(m_wordCountLabel);
    statusBar()->addPermanentWidget(m_lineCountLabel);
    statusBar()->addPermanentWidget(m_cursorPosLabel);
}

void MainWindow::setupSpellChecker()
{
    m_spellChecker = new SpellChecker(this);
    m_spellChecker->loadWordList("data/words.txt");
    m_highlighter = new SpellCheckerHighlighter(m_spellChecker, m_editor->document());
}

// ─────────────────────────────────────────────
// File operations (with exception handling)
// ─────────────────────────────────────────────

void MainWindow::open_file(const QString& path)
{
    QFile file(path);
    if (!file.exists())
        throw file_not_found_exception(path.toStdString());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw file_read_exception(path.toStdString());

    QTextStream in(&file);
    m_editor->setPlainText(in.readAll());
}

void MainWindow::save_file(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw file_write_exception(path.toStdString());

    QTextStream out(&file);
    out << m_editor->toPlainText();
}

// ─────────────────────────────────────────────
// File menu slots
// ─────────────────────────────────────────────

void MainWindow::newFile()
{
    if (!maybeSave())
        return;

    m_editor->clear();
    m_currentFile.clear();
    m_modified = false;
    update_title();
}

void MainWindow::openFile()
{
    if (!maybeSave())
        return;

    QString path = QFileDialog::getOpenFileName(this, "Open File", "",
        "Text Files (*.txt);;HTML Files (*.html *.htm);;All Files (*)");

    if (path.isEmpty())
        return;

    try {
        open_file(path);
        m_currentFile = path;
        m_modified = false;
        update_title();
    } catch (const notepad_exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::saveFile()
{
    if (m_currentFile.isEmpty()) {
        saveFileAs();
        return;
    }

    try {
        save_file(m_currentFile);
        m_modified = false;
        update_title();
    } catch (const notepad_exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::saveFileAs()
{
    QString path = QFileDialog::getSaveFileName(this, "Save File As", "",
        "Text Files (*.txt);;HTML Files (*.html *.htm);;All Files (*)");

    if (path.isEmpty())
        return;

    try {
        save_file(path);
        m_currentFile = path;
        m_modified = false;
        update_title();
    } catch (const notepad_exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

// ─────────────────────────────────────────────
// Edit menu slots
// ─────────────────────────────────────────────

void MainWindow::undo() { m_editor->undo(); }
void MainWindow::redo() { m_editor->redo(); }
void MainWindow::cut() { m_editor->cut(); }
void MainWindow::copy() { m_editor->copy(); }
void MainWindow::paste() { m_editor->paste(); }
void MainWindow::selectAll() { m_editor->selectAll(); }

// ─────────────────────────────────────────────
// Text transform slots
// ─────────────────────────────────────────────

void MainWindow::applyTransform(std::function<QString(const QString&)> transform)
{
    QTextCursor cursor = m_editor->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::Document);

    cursor.insertText(transform(cursor.selectedText()));
}

void MainWindow::toUpperCase() { applyTransform(::toUpperCase); }
void MainWindow::toLowerCase() { applyTransform(::toLowerCase); }
void MainWindow::toCapitalized() { applyTransform(::toCapitalized); }
void MainWindow::toSentenceCase() { applyTransform(::toSentenceCase); }
void MainWindow::toSwapCase() { applyTransform(::toSwapCase); }

// ─────────────────────────────────────────────
// Rich text formatting
// ─────────────────────────────────────────────

void MainWindow::toggleBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(m_editor->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    m_editor->mergeCurrentCharFormat(fmt);
}

void MainWindow::toggleItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(!m_editor->fontItalic());
    m_editor->mergeCurrentCharFormat(fmt);
}

void MainWindow::toggleUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(!m_editor->fontUnderline());
    m_editor->mergeCurrentCharFormat(fmt);
}

// ─────────────────────────────────────────────
// Optional: Font dialog
// ─────────────────────────────────────────────

void MainWindow::openFontDialog()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, m_editor->currentFont(), this, "Select Font");

    if (!ok)
        return;

    QTextCursor cursor = m_editor->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat fmt;
        fmt.setFont(font);
        cursor.mergeCharFormat(fmt);
    } else {
        m_editor->setCurrentFont(font);
    }
}

// ─────────────────────────────────────────────
// Optional: Color picker
// ─────────────────────────────────────────────

void MainWindow::openColorPicker()
{
    QColor color = QColorDialog::getColor(m_editor->textColor(), this, "Select Text Color");

    if (!color.isValid())
        return;

    QTextCharFormat fmt;
    fmt.setForeground(color);
    m_editor->mergeCurrentCharFormat(fmt);
}

// ─────────────────────────────────────────────
// Find / Replace
// ─────────────────────────────────────────────

void MainWindow::openFindReplace()
{
    FindReplaceDialog dialog(this);

    connect(&dialog, &FindReplaceDialog::findNext, this, [this](const QString& text) {
        if (!m_editor->find(text))
            QMessageBox::information(this, "Find", "Text not found.");
    });

    connect(&dialog, &FindReplaceDialog::replace, this, [this](const QString& find, const QString& rep) {
        QTextCursor cursor = m_editor->textCursor();
        if (cursor.hasSelection() && cursor.selectedText() == find)
            cursor.insertText(rep);
        m_editor->find(find);
    });

    connect(&dialog, &FindReplaceDialog::replaceAll, this, [this](const QString& find, const QString& rep) {
        QString content = m_editor->toPlainText();
        int count = content.count(find);
        content.replace(find, rep);
        m_editor->setPlainText(content);
        QMessageBox::information(this, "Replace All", QString("Replaced %1 occurrence(s).").arg(count));
    });

    dialog.exec();
}

// ─────────────────────────────────────────────
// Word Frequency
// ─────────────────────────────────────────────

void MainWindow::openWordFrequency()
{
    QString text = m_editor->toPlainText().toLower();
    QRegularExpression wordRegex("\\b[a-z]+\\b");
    QRegularExpressionMatchIterator it = wordRegex.globalMatch(text);

    QMap<QString, int> freq;
    while (it.hasNext())
        freq[it.next().captured()]++;

    WordFrequencyDialog dialog(freq, this);
    dialog.exec();
}

// ─────────────────────────────────────────────
// Spell check
// ─────────────────────────────────────────────

void MainWindow::checkSpelling()
{
    m_highlighter->rehighlight();
    statusBar()->showMessage("Spell check complete.", 3000);
}

// ─────────────────────────────────────────────
// Status bar
// ─────────────────────────────────────────────

void MainWindow::updateStatusBar()
{
    QString text = m_editor->toPlainText();
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    int wordCount = words.size();
    int lineCount = text.isEmpty() ? 1 : text.count('\n') + 1;

    m_wordCountLabel->setText(QString("Words: %1").arg(wordCount));
    m_lineCountLabel->setText(QString("Lines: %1").arg(lineCount));
}

void MainWindow::updateCursorPosition()
{
    QTextCursor cursor = m_editor->textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    m_cursorPosLabel->setText(QString("Ln %1, Col %2").arg(line).arg(col));
}

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

bool MainWindow::maybeSave()
{
    if (!m_modified)
        return true;

    auto btn = QMessageBox::warning(this, "Notepad",
        "The document has been modified.\nDo you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (btn == QMessageBox::Save) {
        saveFile();
        return !m_modified;
    }

    return btn == QMessageBox::Discard;
}

void MainWindow::update_title()
{
    if (m_currentFile.isEmpty())
        setWindowTitle("Notepad");
    else
        setWindowTitle("Notepad: " + m_currentFile);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = m_editor->createStandardContextMenu();

    QTextCursor cursor = m_editor->cursorForPosition(m_editor->mapFromGlobal(event->globalPos()));
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();

    if (!word.isEmpty() && !m_spellChecker->isCorrect(word)) {
        QStringList suggestions = m_spellChecker->suggestions(word);
        if (!suggestions.isEmpty()) {
            menu->insertSeparator(menu->actions().first());
            QMenu* suggestMenu = new QMenu("Spelling suggestions", menu);

            for (const QString& suggestion : suggestions) {
                QAction* act = suggestMenu->addAction(suggestion);
                connect(act, &QAction::triggered, this, [this, cursor, suggestion]() mutable {
                    cursor.insertText(suggestion);
                });
            }

            menu->insertMenu(menu->actions().first(), suggestMenu);
        }
    }

    menu->exec(event->globalPos());
    delete menu;
}