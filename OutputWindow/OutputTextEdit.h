#ifndef OUTPUTTEXTEDIT_H
#define OUTPUTTEXTEDIT_H

#include <QPlainTextEdit>
#include <QTextCursor>
#include <QElapsedTimer>
#include <QTimer>

class AnsiEscapeCodeHandler;

class OutputTextEdit : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit OutputTextEdit(QWidget *parent = nullptr);
    ~OutputTextEdit();

    void appendMessage(const QString &text);

    void deleteAfterChars();
    void movePosition(QTextCursor::MoveOperation operation);

    bool isEmpty() const;

protected:
    bool isScrollbarAtBottom() const;

    void paintEvent(QPaintEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

    bool focusNextPrevChild(bool next) override;

signals:
    void input(const QByteArray &data);

private slots:
    void scrollToBottom();

    void onActionCopy();
    void onActionCopyAll();
    void onActionPaster();
    void onActionSelectAll();
    void onActionClean();
    void onActionScrollToBottom();
    void onActionFont();

private:
    QTextCursor m_cursor;
    AnsiEscapeCodeHandler *m_ansiHandler = nullptr;
    QTimer m_scrollTimer;
    QElapsedTimer m_lastMessage;

    QMenu *m_menu = nullptr;
    QAction *m_actionCopy = nullptr;
    QAction *m_actionCopyAll = nullptr;
    QAction *m_actionPaster = nullptr;
    QAction *m_actionSelectAll = nullptr;
    QAction *m_actionClean = nullptr;
    QAction *m_actionScrollToBottom = nullptr;
    QAction *m_actionFont = nullptr;

    QTimer m_cursorBlink;
    bool m_blink = false;
};

#endif // OUTPUTTEXTEDIT_H
