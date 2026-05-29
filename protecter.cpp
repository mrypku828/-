#include "protecter.h"
#include "button.h"

// 构造函数：防御者
Protecter::Protecter()
{
    hp = 450;        // 血量
    atk = 0;         // 无攻击力
    state = 1;       // 状态1：完好
    //setMovie(":/resources/wallnut1.gif");
}

// 每帧更新：状态、动画、回血、强化
void Protecter::advance(int phase)
{
    if(!phase) return;
    update();

    // 鼠标样式（保留强化 + 铲子）
    if(Button::shovel_activate || Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    // 强化模式：缓慢回血
    if(!state && hp <= 700)
        hp += 0.8;

    // 根据血量切换破损动画
    if(hp <= 250 && state == 1)
    {
        state = 2;
        setMovie(":/resources/wallnut2.gif");
    }
    else if(hp <= 100 && state == 2)
    {
        state = 3;
        setMovie(":/resources/wallnut3.gif");
    }
    // 死亡删除
    else if(hp <= 0 && state == 3)
    {
        delete this;
        return;
    }
}

// 鼠标点击事件（保留铲子 + 强化模式）
void Protecter::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        // 铲子：删除防御者
        if(Button::shovel_activate)
        {
            QGraphicsItem* item = scene()->itemAt(QPoint(798,40), transform());
            Button* button = qgraphicsitem_cast<Button*>(item);
            button->shovel_activate = false;
            button->counter = 0;
            button->state = 0;
            delete this;
        }
        // 强化模式：触发回血状态
        else if(Button::power_activate)
        {
            QGraphicsItem* item = scene()->itemAt(QPoint(918,60), transform());
            Button* button = qgraphicsitem_cast<Button*>(item);
            button->counter = 0;
            button->state = 0;
            button->power_activate = false;

            state = 0; // 进入强化回血状态
        }
    }
}