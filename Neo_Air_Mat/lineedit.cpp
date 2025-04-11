#include "lineedit.h"
LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent)
{
    process = new QProcess(this);
}

LineEdit::LineEdit(const QString& txt, QWidget *parent) : QLineEdit(txt, parent)
{
}

void LineEdit::focusInEvent(QFocusEvent *event)
{
    qDebug() << "focus In";
    QLineEdit::focusInEvent(event);
    virtual_keyboard();
}

void LineEdit::focusOutEvent(QFocusEvent *event)
{
    if(process != nullptr and process->state() == QProcess::Running)
    {
        process->terminate();
        process->waitForFinished();
    }
}

void LineEdit::virtual_keyboard()
{

    //QProcess process(this);
    qDebug() << "QProcess start";
    QStringList arguments;
    process->start("matchbox-keyboard",arguments);
    //connect(process, SIGNAL(finished()), SLOT(deleteLater()));
}

