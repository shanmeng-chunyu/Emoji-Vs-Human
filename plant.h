#ifndef PLANT_H
#define PLANT_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMovie>
#include "bullet.h"
class Plant : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(int flashAlpha READ flashAlpha WRITE setFlashAlpha)
public:
    explicit Plant(int type,int R,int C,QGraphicsPixmapItem *parent = nullptr);
    int Type;//植物种类
    int Health;//生命值
    int BulletType;//子弹类型;
    int PosR;//所在行
    int PosC;//所在列
    QTimer *A_Timer;//攻击倒计时
    int AttackTime=2500;//攻击间隔
    QMovie *M_Movie;//动画效果
    virtual void Stop();//暂停
    virtual void Start();//开始
    int RemainingTime=0;//暂停时剩余时间
    void GetHurt(int damage);//受伤效果
private:
    virtual void Attack();//攻击函数
    void mousePressEvent(QGraphicsSceneMouseEvent *event);//处理点击事件


    QPropertyAnimation *F_Anim;//受伤动画
    int m_flashAlpha = 0;
    int flashAlpha() const { return m_flashAlpha; }
    void setFlashAlpha(int alpha);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void HurtFlash();

signals:
    void clicked();//铲除信号
    void attack(Bullet *bullet);//攻击信号
    void dead(Plant* newPlant);//死亡信号
};

class Laugh : public Plant
{
public:
    explicit Laugh(int X,int Y);
    QTimer *A_Timer2;//第二发子弹的计时器
    void Stop();//第二发额外暂停，开始
    void Start();
private:
    void Attack();
    int RemainingTime2=0;//第二发暂停时的剩余
};

class Stars : public Plant
{
    Q_OBJECT
public:
    explicit Stars(int X,int Y);
private:
    void Attack();//攻击函数重载，产生星星
signals:
    void G_Star(Starb* starb);
};

#endif // PLANT_H
