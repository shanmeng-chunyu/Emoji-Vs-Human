#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include "playscene.h"
#include <QGraphicsScene>
QT_BEGIN_NAMESPACE
namespace Ui {
class StartWindow;
}
QT_END_NAMESPACE

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
signals:
    void newPlayScene();
private:
    Ui::StartWindow *ui;
    void setTitleLabel();
    void setStartButton();
    void paintEvent(QPaintEvent *ev);
    QGraphicsScene *Scene;
    QGraphicsView *View;
    PlayScene *play=nullptr;//游玩窗口
};
#endif // STARTWINDOW_H
