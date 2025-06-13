#ifndef PAUSEBUTTON_H
#define PAUSEBUTTON_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsItem>
class PauseButton : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    PauseButton(const QString &text,QGraphicsItem *parent=nullptr);
private:
    QString m_text;    // 按钮文本
    bool m_isHovered;  // 悬停状态
    bool m_isPressed;  // 按下状态
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
signals:
    void clicked();//点击后处理
};

#endif // PAUSEBUTTON_H
