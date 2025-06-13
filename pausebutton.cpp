#include "pausebutton.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
PauseButton::PauseButton(const QString &text,QGraphicsItem *parent):QGraphicsRectItem(parent),m_text(text),m_isHovered(false), m_isPressed(false)
{
    setRect(0,0,100,40); // 按钮大小
    setAcceptHoverEvents(true); // 允许悬停事件
}

void PauseButton::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    m_isPressed = true;
    update(); // 触发重绘，显示按下状态
    event->accept(); // 阻止事件继续传递
}

void PauseButton::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (m_isPressed && rect().contains(event->pos())) {
        emit clicked(); // 发射点击信号
    }
    m_isPressed = false;
    update();
    event->accept();
}

void PauseButton::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    m_isHovered = true;
    update();
    QGraphicsRectItem::hoverEnterEvent(event);
}
void PauseButton::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    m_isHovered = false;
    update();
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void PauseButton::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor normalColor(165, 42, 42);   // #A52A2A
    QColor hoverColor(139, 69, 19);    // #8B4513
    QColor pressColor(101, 67, 33);    // #654321
    QColor fillColor = normalColor;
    if (m_isPressed) {
        fillColor = pressColor;
    } else if (m_isHovered) {
        fillColor = hoverColor;
    }
    painter->setBrush(fillColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rect(), 5, 5);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    painter->drawText(rect(), Qt::AlignCenter, m_text);
}
