#include "normalenemy.h"
#include "button.h"

// 构造函数：初始化普通敌人属性
NormalEnemy::NormalEnemy()
{
    hp = 100;           // 血量
    atk = 0.3;          // 伤害
    state = WALK;       // 初始状态：行走
    speed = 0.7;        // 移动速度
    slow = NO_SLOW;     // 初始无减速
    time = 120;         // 冰冻恢复倒计时

    // 随机选择行走动画（两个方向）
    if (rand() % 2)
        SetMovie(":/resources/zw1.gif");
    else
        SetMovie(":/resources/zw2.gif");
}

// 每帧更新逻辑
void NormalEnemy::advance(int phase)
{
    if (!phase) return;
    update();

    // 强化模式激活时，鼠标变为手型
    if (Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    // ---------------- 死亡/燃烧处理 ----------------
    if (hp <= 0)
    {
        // 普通死亡（被打死）
        if (state == ATTACK || state == WALK)
        {
            state = DEAD;
            Sethead(":/resources/zh.gif");
            SetMovie(":/resources/zd.gif");
            movie->setSpeed(50);
        }
        // 被燃烧死亡（炸弹等）
        else if (state == BURN)
        {
            state = DEAD;
            speed = 0;
            SetMovie(":/resources/burn.gif");
        }

        // 死亡动画播放完毕，删除对象
        if (state == DEAD && movie->frameCount() == movie->currentFrameNumber() + 1)
        {
            delete this;
        }
        return;
    }

    // ---------------- 强化状态处理 ----------------
    if (super)
    {
        movie->setSpeed(250);  // 动画变快
        speed = 2.5;           // 移动速度提升
        atk = 0.8;             // 伤害提升
    }

    // ---------------- 碰撞检测：攻击炮台 ----------------
    QList<QGraphicsItem*> items = collidingItems();
    if (!items.isEmpty())
    {
        Paotai *plant = qgraphicsitem_cast<Paotai *>(items[0]);
        if (plant)
        {
            plant->hp -= atk; // 扣炮台血量

            // 切换为攻击动画
            if (state != ATTACK)
            {
                SetMovie(":/resources/za.gif");
                state = ATTACK;
            }
            return; // 啃食中，不移动
        }
    }

    // ---------------- 减速/冰冻处理 ----------------
    if (slow)
    {
        // 冰冻倒计时结束，解除减速
        if (++counter > time)
        {
            snowfree();
            return;
        }

        // 减速1：寒冰子弹效果
        if (slow == 1)
        {
            movie->setSpeed(50);
            speed = 0.35;
        }
        // 减速2：完全冰冻
        else if (slow == 2)
        {
            movie->setSpeed(0);
            speed = 0;
        }
    }

    // ---------------- 状态切换：攻击结束恢复行走 ----------------
    if (state == ATTACK)
    {
        state = WALK;
        if (rand() % 2)
            SetMovie(":/resources/zw1.gif");
        else
            SetMovie(":/resources/zw2.gif");
    }

    // ---------------- 边界检测：走出屏幕左侧删除 ----------------
    if (x() < 100)
    {
        delete this;
        return;
    }

    // 正常移动
    setX(x() - speed);
}

// 解除冰冻/减速状态
void NormalEnemy::snowfree()
{
    slow = NO_SLOW;       // 取消减速标记
    speed = 0.7;          // 恢复原速
    movie->setSpeed(100); // 恢复正常动画速度
}

// 鼠标点击事件：触发强化模式
void NormalEnemy::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos(918, 60);
    QGraphicsItem* item = scene()->itemAt(pos, QTransform());
    Button* button = qgraphicsitem_cast<Button*>(item);

    if (button)
    {
        // 左键点击 + 强化模式激活
        if (event->button() == Qt::LeftButton && button->power_activate)
        {
            setCursor(Qt::PointingHandCursor);
            button->power_activate = false;
            button->counter = 0;
            button->state = 0;

            snowfree(); // 解除所有减速
            super = true; // 开启超级模式
            qDebug() << "power normal enemy activated";
        }
    }
}