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
    void insertMessage(const QString &text, bool keepCursor);
    void overwriteMessage(const QString &text);

    void setLocalEchoEnable(bool enable);

    void deletePreviousChar();
    void deleteAfterChars();
    void movePosition(QTextCursor::MoveOperation operation);

    bool isEmpty() const;

protected:
    bool isScrollbarAtBottom() const;
    void moveCursorToEnd();

    void paintEvent(QPaintEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

signals:
    void getData(const QByteArray &data);

private slots:
    void scrollToBottom();

    void onActionCopy();
    void onActionCopyAll();
    void onActionSelectAll();

private:
    QTextCursor m_cursor;
    AnsiEscapeCodeHandler *m_ansiHandler = nullptr;
    QTimer m_scrollTimer;
    QElapsedTimer m_lastMessage;
    bool m_localEchoEnabled = false;

    QMenu *m_menu = nullptr;
    QAction *m_actionCopy = nullptr;
    QAction *m_actionCopyAll = nullptr;
    QAction *m_actionSelectAll = nullptr;
};

#endif // OUTPUTTEXTEDIT_H
