#ifndef BRICK_H
#define BRICK_H

#include <QGraphicsPixmapItem>

class Brick : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Brick(int X,int Y,QGraphicsPixmapItem *parent = nullptr);
    bool IsPlant=false;//是否已经种植
    bool IsPause=false;//是否暂停（暂停时无法种植）
    int PosX=0;
    int PosY=0;
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);//处理点击事件
signals:
    void clicked(int PosX,int PosY);
};

#endif // BRICK_H
