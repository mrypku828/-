#include "plusenemy.h"
#include "button.h"


PlusEnemy::PlusEnemy()
{
    hp = 350;            // 高血量
    atk = 0.5;           // 攻击伤害
    state = WALK;        // 初始状态：行走
    speed = 0.7;         // 移动速度
    slow = NO_SLOW;      // 无减速
    time = 120;          // 减速持续时间

    // 设置行走动画
    SetMovie(":/resources/czbw.gif");
}

// 每帧逻辑更新：移动、攻击、死亡、减速、动画切换
void PlusEnemy::advance(int phase)
{
    if (!phase) return;
    update();

    // 强化模式光标
    if (Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    // ===================== 死亡处理 =====================
    if (hp <= 0)
    {
        // 普通死亡
        if (state == ATTACK || state == WALK)
        {
            state = DEAD;
            Sethead(":/resources/zh.gif");
            SetMovie(":/resources/zd.gif");
            movie->setSpeed(50);
        }
        // 燃烧死亡
        else if (state == BURN)
        {
            state = DEAD;
            speed = 0;
            SetMovie(":/resources/burn.gif");
        }

        // 死亡动画播放完 → 删除对象
        if (state == DEAD && movie->frameCount() == movie->currentFrameNumber() + 1)
        {
            delete this;
        }
        return;
    }

    // ===================== 超级强化模式 =====================
    if (super)
    {
        movie->setSpeed(250);
        speed = 2.5;
        atk = 0.8;
    }

    // ===================== 碰撞植物 → 攻击 =====================
    QList<QGraphicsItem*> items = collidingItems();
    if (!items.isEmpty())
    {
        Paotai *plant = qgraphicsitem_cast<Paotai *>(items[0]);
        if (plant)
        {
            plant->hp -= atk;

            // 切换攻击动画
            if (state != ATTACK)
            {
                SetMovie(":/resources/czba.gif");
                state = ATTACK;
            }
            return;
        }
    }

    // ===================== 攻击结束 → 恢复行走 =====================
    if (state == ATTACK)
    {
        state = WALK;
        SetMovie(":/resources/czbw.gif");
    }

    // ===================== 减速/冰冻处理 =====================
    if (slow)
    {
        if (++counter > time)
        {
            snowfree();
            return;
        }

        // 减速状态1
        if (slow == 1)
        {
            movie->setSpeed(50);
            speed = 0.35;
        }
        // 冰冻状态2
        else if (slow == 2)
        {
            movie->setSpeed(0);
            speed = 0;
        }
    }

    // ===================== 走出屏幕左侧 → 移除 =====================
    if (x() < 100) {
        delete this;
        return;
    }

    // 正常向左移动
    setX(x() - speed);
}

// 解除减速/冰冻状态
void PlusEnemy::snowfree()
{
    slow = NO_SLOW;       // 清除减速标记
    speed = 0.7;          // 恢复正常速度
    movie->setSpeed(100); // 恢复动画速度
}

// 鼠标点击：触发强化模式
void PlusEnemy::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos(918, 60);
    QGraphicsItem* item = scene()->itemAt(pos, QTransform());
    Button* button = qgraphicsitem_cast<Button*>(item);

    if (button)
    {
        if (event->button() == Qt::LeftButton && button->power_activate)
        {
            setCursor(Qt::PointingHandCursor);
            button->power_activate = false;
            button->counter = 0;
            button->state = 0;

            snowfree();    // 解除减速
            super = true;  // 开启强化
            qDebug() << "power plus enemy activated";
        }
    }
}