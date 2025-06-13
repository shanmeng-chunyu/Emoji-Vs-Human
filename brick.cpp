#include "brick.h"
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>
#include <QGraphicsItem>
Brick::Brick(int X,int Y,QGraphicsPixmapItem *parent)
    : QGraphicsPixmapItem{parent}
{
    PosX=X;
    PosY=Y;
    setPixmap(QPixmap(":/others/resource/others/Brick.png"));
    setScale(0.66);
    this->setOpacity(0.01);//设置成基本透明，保留点击事件
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Brick::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(IsPlant || IsPause){
        QGraphicsPixmapItem::mousePressEvent(event);
        return;//已经种植植物，直接返回
    }
    if (event->button() == Qt::LeftButton){
        emit clicked(PosX,PosY);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}
