#include "bullet.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QEasingCurve>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsItem>
#include <QCursor>
Bullet::Bullet()
{
    setPixmap(QPixmap(":/Bullet/resource/BulletPNG/Drew.png"));
    setScale(0.3);
}

Starb::Starb():Bullet(){
    setPixmap(QPixmap(":/others/resource/others/Star.png"));
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void Starb::setupAnimation(qreal X,qreal Y){
    m_animation = new QPropertyAnimation(this, "pos");

    // 动画参数配置
    m_animation->setDuration(1500);          // 总时长1.5秒
    m_animation->setEasingCurve(QEasingCurve::OutBounce); // 弹跳曲线
    m_animation->setKeyValueAt(0.3, QPointF(X+10,Y-5)); // 上升至最高点
}

void Starb::startBounce(qreal X,qreal Y){
    // 设置动画路径
    m_animation->setStartValue(QPointF(X, Y));
    m_animation->setEndValue(QPointF(X+35, Y+30));
    m_animation->start();

}

void Starb::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        event->accept();  // 标识事件已被处理
        emit Pickup();  // 触发信号
        return;
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void Starb::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    setCursor(Qt::PointingHandCursor);  // 悬停时显示手型光标
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void Starb::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    setCursor(Qt::ArrowCursor);  // 恢复默认光标
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

Coinb::Coinb():Starb(){
    setPixmap(QPixmap(":/others/resource/others/Coin.png"));
    setScale(0.9);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

Blood::Blood():Bullet(){
    setPixmap(QPixmap(":/Bullet/resource/BulletPNG/Blood.png"));
    damage=15;
}

Heart::Heart():Bullet(){
    setPixmap(QPixmap(":/Bullet/resource/BulletPNG/Heart.png"));
    damage=20;
}

Snow::Snow():Bullet(){
    setPixmap(QPixmap(":/Bullet/resource/BulletPNG/Snow.png"));
    damage=20;
}

Rocketb::Rocketb():Bullet(){
    setPixmap(QPixmap(":/Props/resource/PropPNG/Rocket.png"));
    setScale(0.4);
    damage=5000;
}
