#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QEvent>
#include <QDebug>
#include <QProcess>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = nullptr);
    LineEdit(const QString&, QWidget *parent = nullptr);
    QProcess *process;

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    bool _moved = false;
    int _lastDiff = 0;
    QWidget *container;

    void virtual_keyboard();
};

#endif // LINEEDIT_H
