#ifndef INFORMATION_H
#define INFORMATION_H

#include <QObject>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>

#include "graph.h"

class information : public QObject
{
    Q_OBJECT
public:
    explicit information(QComboBox* birth,QComboBox* tall,QComboBox* kg, QRadioButton* male, QRadioButton* female, QPushButton* btn,QObject *parent = nullptr);
    bool check_btn();
    void set_check_btn(bool is_btn);
    void set_2_btn(QComboBox* birth,QComboBox* tall,QComboBox* kg, QRadioButton* male, QRadioButton* female, QPushButton* btn);
    QString get_gender();

    void set_range_kPa(QLineEdit* one,     QLineEdit* two_1,  QLineEdit* two_2,  QLineEdit* three_1,
                       QLineEdit* three_2, QLineEdit* four_1, QLineEdit* four_2, QLineEdit* five_1,
                       QLineEdit* five_2,  QLineEdit* six_1,  QLineEdit* six_2,  QLineEdit* seven,
                       QLineEdit* kPa,     graph* heatmap,    QPushButton* ok);

    bool check_set_btn();
    void set_sett_btn(bool is_btn);

private slots:
    void check_answer();
    void modi_info();
    void modi_gender();
    //void modi_set();

private:
    QComboBox*    birth_;
    QComboBox*    tall_;
    QComboBox*    kg_;
    QRadioButton* male_;
    QRadioButton* female_;
    QComboBox*    birth2_;
    QComboBox*    tall2_;
    QComboBox*    kg2_;
    QRadioButton* male2_;
    QRadioButton* female2_;
    QPushButton*  submit_;
    QPushButton*  modi_;
    QString      gender;

    QLineEdit* one;
    QLineEdit* two_1;
    QLineEdit* two_2;
    QLineEdit* three_1;
    QLineEdit* three_2;
    QLineEdit* four_1;
    QLineEdit* four_2;
    QLineEdit* five_1;
    QLineEdit* five_2;
    QLineEdit* six_1;
    QLineEdit* six_2;
    QLineEdit* seven;
    QLineEdit* kPa;

    graph* graphs;
    QPushButton* set_apply;

    bool is_btn = false;
    bool is_gen = false;

    bool is_set_btn = false;

signals:

};

#endif // INFORMATION_H
