#ifndef GRAPH_H
#define GRAPH_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QVector>
#include <bitset>

#define SAFE_ACCESS_VECTOR(vec,index)\
    ((index) >= 0 && (index) < (vec).size() ? (vec)[index] : (qDebug() << "This", (vec)[0]))

class graph : public QGraphicsView
{
public:
    graph(QWidget *parent = nullptr);
    QGraphicsScene* update_map(bool is_char);
    QGraphicsScene* initialize_graph();
    QGraphicsScene* detail_map();
    void get_data(QMap<QString,QString> datas);
    void set_range(float one, float two_1, float two_2, float three_1,
                   float three_2, float four_1, float four_2, float five_1,
                   float five_2, float six_1, float six_2, float seven_1);
    bool get_trigger();
    void set_kPa(float pressure);
    std::bitset<28> get_kPa_bit();

private:
    QGraphicsScene *scene;
    QMap<QString,QString> heatmap;
    struct map_struct{
        QVector<QColor> colors = {Qt::blue,Qt::blue,Qt::blue,Qt::blue};
        QVector<float> pressure;
    };
    QVector<QGraphicsRectItem*> rect_item;
    QVector<QGraphicsTextItem*> text_item;

    bool is_kPa  = false;
    float  kPa     = 1;
    int  kPa_cnt = 0;
    int  spot    = 0;

    //void create_map(int start_x, int start_y, int rect_size, int space, QString press);
    void create_map(bool is_char);
    QVector<QColor> cal_color(QString press);
    map_struct parse_data(QString press);

    // 정사각형 크기와 간격 설정
    const int rectSize = 35;
    const int spacing = 5; // 정사각형 사이 간격
    const int columns = 4;//8
    const int rows = 7;//14

    float one     = 0.0;
    float two_1   = 0.1;
    float two_2   = 0.5;
    float three_1 = 0.6;
    float three_2 = 1.0;
    float four_1  = 1.1;
    float four_2  = 1.5;
    float five_1  = 1.6;
    float five_2  = 2.0;
    float six_1   = 2.1;
    float six_2   = 2.5;
    float seven_1 = 2.6;

    void set_trigger(bool is);

    std::bitset<28> is_open;

};

#endif // GRAPH_H
