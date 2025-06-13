#include "card.h"
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsItem>
#include <QCursor>
#include <QDebug>
#include <QPainter>
Card::Card(int CardType,QGraphicsPixmapItem *parent)
    : QGraphicsPixmapItem{parent}
{
    this->CardType=CardType;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    switch (CardType){
    case 0:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Sweat.png"));
        setOpacity(0.01);
        Star_n=0;
        break;
    case 1:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Sweat.png"));
        Star_n=50;
        break;
    case 2:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Stars.png"));
        Star_n=50;
        break;
    case 3:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Love.png"));
        Star_n=75;
        break;
    case 4:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Hot.png"));
        Star_n=75;
        break;
    case 5:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Cold.png"));
        Star_n=75;
        break;
    case 6:
        setPixmap(QPixmap(":/Card/resource/CardPNG/Laugh.png"));
        Star_n=100;
        break;
    case 7:
        setPixmap(QPixmap(":/Props/resource/PropPNG/Rocket.png"));
        Coin_n=10;
        setScale(0.5);
        return;
    case 8:
        setPixmap(QPixmap(":/Props/resource/PropPNG/Suger.png"));
        Coin_n=5;
        setScale(0.5);
        return;
    }
    this->setScale(0.8);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if (event->button() == Qt::LeftButton && Selectable) {
        event->accept();  // 标识事件已被处理
        emit clicked(CardType);  // 触发信号
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}
void Card::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    if(Selectable){
        setCursor(Qt::PointingHandCursor);  // 悬停时显示手型光标
    }
    QGraphicsPixmapItem::hoverEnterEvent(event);
}
void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    setCursor(Qt::ArrowCursor);  // 恢复默认光标
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void Card::setDisabledState(bool disabled){
    if (m_disabled != disabled) {
        m_disabled = disabled;
        update(); // 触发重绘
    }
}

void Card::paint(QPainter* painter,const QStyleOptionGraphicsItem* option,QWidget* widget){
    QGraphicsPixmapItem::paint(painter, option, widget);
    // 2. 叠加灰色效果
    if (m_disabled) {
        painter->save();
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->fillRect(boundingRect(), QColor(128, 128, 128, 128)); // 透明度50%的灰色
        painter->restore();
    }
}//灰色效果显示
