#include "information.h"

information::information(QComboBox* birth,QComboBox* tall,QComboBox* kg, QRadioButton* male, QRadioButton* female,QPushButton* btn,QObject *parent)
    : QObject(parent), birth_(birth),tall_(tall), kg_(kg), male_(male), female_(female), submit_(btn)
{
    connect(tall_,SIGNAL(currentTextChanged(QString)),this,SLOT(check_answer()));
    connect(birth_,SIGNAL(currentTextChanged(QString)),this,SLOT(check_answer()));
    connect(kg_,SIGNAL(currentTextChanged(QString)),this,SLOT(check_answer()));
    connect(male_,SIGNAL(toggled(bool)),this,SLOT(check_answer()));
    connect(female_,SIGNAL(toggled(bool)),this,SLOT(check_answer()));
}

void information::set_range_kPa(QLineEdit *one, QLineEdit *two_1, QLineEdit *two_2, QLineEdit *three_1, QLineEdit *three_2, QLineEdit *four_1, QLineEdit *four_2, QLineEdit *five_1, QLineEdit *five_2, QLineEdit *six_1, QLineEdit *six_2, QLineEdit *seven, QLineEdit *kPa, graph *heatmap, QPushButton *ok)
{
    this->one = one;
    this->two_1 = two_1;
    this->two_2 = two_2;
    this->three_1 = three_1;
    this->three_2 = three_2;
    this->four_1 = four_1;
    this->four_2 = four_2;
    this->five_1 = five_1;
    this->five_2 = five_2;
    this->six_1 = six_1;
    this->six_2 = six_2;
    this->seven = seven;
    this->kPa = kPa;
    this->graphs = heatmap;
    this->set_apply = ok;
    graphs->set_kPa(kPa->text().toFloat());
    set_apply->setDisabled(false);
    set_sett_btn(true);
    //set_apply->setStyleSheet("background-color:rgb(0,160,233);color:rgb(255, 255, 255);border:solid 1px;");
    graphs->set_range(one->text().toFloat(),two_1->text().toFloat(),two_2->text().toFloat(),
                      three_1->text().toFloat(),three_2->text().toFloat(),four_1->text().toFloat(),
                      four_2->text().toFloat(),five_1->text().toFloat(),five_2->text().toFloat(),
                      six_1->text().toFloat(),six_2->text().toFloat(),seven->text().toFloat());

//    connect(this->one,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->two_1,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->two_2,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->three_1,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->three_2,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->four_1,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->four_2,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->five_1,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->five_2,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->six_1,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->six_2,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->seven,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));
//    connect(this->kPa,SIGNAL(textEdited(QString)),this,SLOT(modi_set()));

}

void information::set_sett_btn(bool is_btn)
{
    this->is_set_btn = is_btn;
}

bool information::check_set_btn()
{
    return is_set_btn;
}
//void information::modi_set()
//{
//    set_apply->setDisabled(false);
//    set_sett_btn(true);
//    set_apply->setStyleSheet("background-color:rgb(0,160,233);color:rgb(255, 255, 255);border:solid 1px;");
//    graphs->set_range(one->text().toFloat(),two_1->text().toFloat(),two_2->text().toFloat(),
//                      three_1->text().toFloat(),three_2->text().toFloat(),four_1->text().toFloat(),
//                      four_2->text().toFloat(),five_1->text().toFloat(),five_2->text().toFloat(),
//                      six_1->text().toFloat(),six_2->text().toFloat(),seven->text().toFloat());
//}

bool information::check_btn()
{
    return is_btn;
}

QString information::get_gender()
{
    return gender;
}

void information::set_check_btn(bool is_btn)
{
    this->is_btn = is_btn;
}
void information::set_2_btn(QComboBox *birth, QComboBox *tall, QComboBox *kg, QRadioButton *male, QRadioButton *female, QPushButton *btn)
{
    this->birth2_ = birth;
    this->tall2_  = tall;
    this->kg2_    = kg;
    this->male2_  = male;
    this->female2_= female;
    this->modi_   = btn;

    connect(tall2_,SIGNAL(activated(int)),this,SLOT(modi_info()));
    connect(birth2_,SIGNAL(activated(int)),this,SLOT(modi_info()));
    connect(kg2_,SIGNAL(activated(int)),this,SLOT(modi_info()));
    connect(male2_,SIGNAL(toggled(bool)),this,SLOT(modi_gender()));
    connect(female2_,SIGNAL(toggled(bool)),this,SLOT(modi_gender()));
}
void information::check_answer()
{
    bool is_tall   = tall_->currentText() != "";
    bool is_birth  = birth_->currentText() != "";
    bool is_kg     = kg_->currentText() != "";
    bool is_gender = male_->isChecked() || female_->isChecked();

    if(is_tall && is_birth && is_kg && is_gender)
    {
        male2_->blockSignals(true);
        female2_->blockSignals(true);

        submit_->setStyleSheet("background-color:rgb(0,160,233);color:rgb(255, 255, 255);border:solid 1px;");
        is_gen = false;
        birth2_->setCurrentIndex(birth_->currentIndex());
        tall2_->setCurrentIndex(tall_->currentIndex());
        kg2_->setCurrentIndex(kg_->currentIndex());
        if(male_->isChecked())
        {
            male2_->setChecked(true);
            male2_->blockSignals(false);
            female2_->blockSignals(false);
            gender = "male";
        }
        else
        {
            female2_->setChecked(true);
            male2_->blockSignals(false);
            female2_->blockSignals(false);
            gender = "female";
        }
        is_btn = true;
    }
    else
    {
        submit_->setStyleSheet("background-color: rgb(217, 217, 217);color:rgb(255, 255, 255);border:solid 1px;");
        is_btn = false;
    }
}
void information::modi_gender()
{
    this->is_gen = true;
    modi_info();
}
void information::modi_info()
{
    bool is_tall   = tall_->currentText() != tall2_->currentText();
    bool is_birth  = birth_->currentText() != birth2_->currentText();
    bool is_kg     = kg_->currentText() != kg2_->currentText();
    bool is_gender = is_gen;

    if(is_tall || is_birth || is_kg || is_gender)
    {
        is_btn = true;
        modi_->setStyleSheet("QPushButton{border: none;border-radius: 0px;background-color:rgb(0,160,233);color:white;} QPushButton:pressed{border: none;border-radius: 0px;background-color:blue;color:white;}");
        if(male2_->isChecked())
        {
            gender = "male";
        }
        else
        {
            gender = "female";
        }
    }
    else
    {

    }
}
