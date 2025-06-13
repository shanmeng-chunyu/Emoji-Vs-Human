#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "human.h"
class Bullet : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Bullet();
    int damage=20;
signals:
    void destoryed();//击中僵尸时销毁
};

class Starb : public Bullet
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit Starb();
    void startBounce(qreal X,qreal Y);//动画曲线
    void setupAnimation(qreal X,qreal Y);//动画曲线
private:
    QPropertyAnimation* m_animation;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);//处理点击事件
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);//处理移入事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);//处理移出事件
signals:
    void Pickup();//处理星星被点击时的情况
};

class Coinb : public Starb
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit Coinb();
};

class Blood : public Bullet
{
public:
    explicit Blood();
};

class Heart : public Bullet
{
public:
    explicit Heart();
    QVector<Human*> A_Human;//记录已经穿透的僵尸，不再造成伤害
};

class Snow: public Bullet
{
public:
    explicit Snow();
};

class Rocketb: public Bullet
{
public:
    explicit Rocketb();
};

#endif // BULLET_H
