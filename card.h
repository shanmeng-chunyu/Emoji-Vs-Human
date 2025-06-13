#ifndef CARD_H
#define CARD_H

#include <QGraphicsPixmapItem>

class Card :public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Card(int CardType,QGraphicsPixmapItem *parent = nullptr);
    int CardType;//当前卡片表示的类型
    int Star_n=0;//需要的星星数
    int Coin_n=0;//需要的金币数
    void setDisabledState(bool disabled);//灰色显示
    bool Selectable=0;
private:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);//处理点击事件
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);//处理移入事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);//处理移出事件
    bool m_disabled = false;//不可用标志（true不可用，flase可用）
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget);
signals:
    void clicked(int type);
};

#endif // CARD_H
