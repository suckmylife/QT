#include "graph.h"

graph::graph(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    is_open.reset();
}

void graph::set_range(float one, float two_1, float two_2, float three_1, float three_2, float four_1, float four_2, float five_1, float five_2, float six_1, float six_2, float seven_1)
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
    this->six_1  = six_1;
    this->six_2  = six_2;
    this->seven_1 = seven_1;
}

void graph::set_kPa(float pressure)
{
    this->kPa = pressure;
}

 void graph::get_data(QMap<QString,QString> datas)
{
    //if(datas.size()>27)
    //{
        this->heatmap = datas;
    //}
}

void graph::set_trigger(bool is)
{
    this->is_kPa = is;
}

bool graph::get_trigger()
{
    return is_kPa;
}

std::bitset<28> graph::get_kPa_bit()
{
    return is_open;
}

graph::map_struct graph::parse_data(QString press)
{
    map_struct comb_data;
    QVector<float> set_color;
    bool is_press = (press[11] == "1") ? true : false;
    QVector<QColor> colors = {Qt::blue,Qt::blue,Qt::blue,Qt::blue};
    QString pressure = press.mid(12, 16);//헤더와 테일을 제외한 압력 값만 파싱

    if(!pressure.contains("7ee7") and is_press) // 헤더가 있으면 안되니까 거를 수 있도록 조건부
    {
        for (int j = 0; j < pressure.size(); j += 4)
        {
            QString hexValue = pressure.mid(j, 4);
            comb_data.pressure.append(static_cast<float>(hexValue.toInt(nullptr, 16)) / 10);
        }

        for(int cnt = 0; cnt<comb_data.colors.size(); ++cnt)
        {
            float value = comb_data.pressure[cnt];
            //int error = SAFE_ACCESS_VECTOR(comb_data.pressure,cnt);
            //qDebug() << "Error : " << error;

            //(value >= kPa) ? is_kPa = true : is_kPa = false; //압력 감지

            if(value <= one)
            {
                comb_data.colors[cnt] = QColor(Qt::gray);
            }
            else if(value >= two_1 and value <= two_2)
            {
                QColor custom("#D8EAFF");
                comb_data.colors[cnt] = QColor("#D8EAFF");
            }
            else if(value >= three_1 and value <= three_1)
            {
                QColor custom("#437CBE");
                comb_data.colors[cnt] = QColor("#437CBE");
            }
            else if(value >= four_1 and value <= four_2)
            {
                QColor custom("#66C580");
                comb_data.colors[cnt] = QColor("#66C580");
            }
            else if(value >= five_1 and value <= five_2)
            {
                QColor custom("#FFD335");
                comb_data.colors[cnt] = QColor("#FFD335");
            }
            else if(value >= six_1 and value <= six_2)
            {
                QColor custom("#FF8F28");
                comb_data.colors[cnt] = QColor("#FF8F28");
            }
            else if(value >= seven_1)
            {
                QColor custom("#FF3333");
                comb_data.colors[cnt] = QColor("#FF3333");
            }
        }
    }

    return comb_data;
}

void graph::create_map(bool is_char)
{
    map_struct comb_data;
    for (int i = 0; i < 28; ++i)
    {
        int group = (i / 4) * 4 + 4;
        int val   = group - (i % 4);
        if(heatmap.contains(QString("%1").arg(val)))
        {
            comb_data = parse_data(heatmap[QString("%1").arg(val)]);
            int num = 0;
            for(int j = 4*i; j < 4*i+4; ++j)
            {
                QGraphicsRectItem* rect = rect_item[j];
                QGraphicsTextItem* text = text_item[j];

                rect->setPen(QPen(comb_data.colors[num]));
                rect->setBrush(QBrush(comb_data.colors[num]));
                int kPa_data = comb_data.pressure[num];
                (kPa_data >= kPa) ? is_kPa = true : is_kPa = false;

                if(is_char)
                {
                    text->setVisible(true);
                    text->setPlainText(QString::number(comb_data.pressure[num]));
                }
                else
                {
                    text->setVisible(false);
                }
                num++;

            }
            set_trigger(is_kPa);

            if(is_kPa)
            {
                is_open.set(val-1);
            }else
            {
                is_open.reset(val-1);
            }
            //qDebug() << val-1 << " : " <<is_open[val-1] << "\n";

//            if(kPa_cnt>26)
//            {
//                kPa_cnt = 0;
//            }
//            else
//            {
//                kPa_cnt++;
//            }

        }
    }
}

QGraphicsScene* graph::initialize_graph()
{
    int x_;
    int y_;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            int x = j * (2 * (rectSize + spacing));
            int y = i * (2 * (rectSize + spacing));
            for (int i_ = 0; i_ < 2; ++i_) {
                for (int j_ = 0; j_ < 2; ++j_) {
                    if(i_ == 1 && j_ == 0)
                    {
                        x_ = x + (rectSize + spacing);
                        y_ = y + (rectSize + spacing);
                    }
                    else if(i_ == 1 && j_ == 1)
                    {
                        x_ = x;
                        y_ = y + (rectSize + spacing);
                    }
                    else
                    {
                        x_ = x + j_ * (rectSize + spacing);
                        y_ = y + i_ * (rectSize + spacing);

                    }
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x_,y_,rectSize,rectSize);
                    rect->setPen(QPen(Qt::blue));
                    rect->setBrush(QBrush(Qt::blue));
                    //rect->setVisible(false);
                    scene->addItem(rect);
                    rect_item.append(rect);

                    // 텍스트 아이템 생성 및 추가
                    QGraphicsTextItem* text = new QGraphicsTextItem("0.0");
                    text->setDefaultTextColor(Qt::white);
                    text->setVisible(false);
                    // 텍스트 위치를 정사각형 중앙으로 조정
                    QRectF rectBounds = rect->boundingRect();
                    QPointF rectCenter = rectBounds.center();
                    text->setPos(rectCenter.x() - text->boundingRect().width() / 2,
                                 rectCenter.y() - text->boundingRect().height() / 2);
                    scene->addItem(text);
                    text_item.append(text);
                }
            }

        }
    }


    return scene;
}

QGraphicsScene* graph::update_map(bool is_char)
{
    // 정사각형 그래프 생성
    create_map(is_char);
    scene->invalidate();

    return scene;
}

QGraphicsScene* graph::detail_map()
{
    // 정사각형 크기와 간격 설정
    const int rectSize = 35;
    const int spacing = 5; // 정사각형 사이 간격
    const int columns = 8;
    const int rows = 14;

    // 정사각형 그래프 생성
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // 각 정사각형의 위치를 간격을 고려하여 설정
            int x = j * (rectSize + spacing);
            int y = i * (rectSize + spacing);

            // 정사각형 아이템 생성 및 추가
            QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, rectSize, rectSize);

            // 텍스트 아이템 생성 및 추가
            QGraphicsTextItem* text = new QGraphicsTextItem("0.0");
            text->setDefaultTextColor(Qt::white);

            // 텍스트 위치를 정사각형 중앙으로 조정
            QRectF rectBounds = rect->boundingRect();
            QPointF rectCenter = rectBounds.center();
            text->setPos(rectCenter.x() - text->boundingRect().width() / 2,
                         rectCenter.y() - text->boundingRect().height() / 2);
            scene->addItem(text);
        }
    }
    return scene;
}
