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

    void appendMessage(const QString &out);

    void setLocalEchoEnable(bool enable);

protected:
    bool isScrollbarAtBottom() const;
    void moveCursorToEnd();

    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

signals:
    void getData(const QByteArray &data);

private slots:
    void scrollToBottom();

    void onActionCopy();
    void onActionSelectAll();

private:
    QTextCursor m_cursor;
    AnsiEscapeCodeHandler *m_ansiHandler = nullptr;
    QTimer m_scrollTimer;
    QElapsedTimer m_lastMessage;
    bool m_localEchoEnabled = false;

    QMenu *m_menu = nullptr;
};

#endif // OUTPUTTEXTEDIT_H
