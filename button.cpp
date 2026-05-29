#include "button.h"
#include "enemy.h"

// 全局静态变量：强化模式是否激活
bool Button::power_activate = false;//----------->待修改
// 全局静态变量：铲子是否激活
bool Button::shovel_activate = false;

// 按钮构造函数：初始化按钮属性
Button::Button(int t)
{
    counter = 0;        // 冷却计时器初始为0
    cool_time = 100;    // 总冷却时间100帧
    state = 0;          // 初始状态：冷却中（不可点击）
    type = t;           // 设置按钮类型（FREEZE/POWER/SHOVEL）
}

// 每帧更新：处理冷却、光标样式
void Button::advance(int phase)
{
    if (!phase) return; // 固定写法，只执行一次逻辑
    update();           // 刷新按钮画面

    if (state) // 状态=1（可点击）
    {
        setCursor(Qt::PointingHandCursor); // 鼠标变成小手
    }
    else // 状态=0（冷却中）
    {
        setCursor(Qt::ArrowCursor); // 正常箭头
        if (++counter > cool_time)  // 计时结束
            state = 1;              // 冷却完成，变为可点击
    }
}

// 绘制按钮：显示图片、文字、冷却遮罩
void Button::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::green);
    QFont font("Calibri", 18, QFont::Bold, true);
    painter->setFont(font);

    // -------- 绘制 冰冻按钮 --------
    if (type == FREEZE)
    {
        painter->drawPixmap(QRect(-80, -20, 160, 40), QPixmap(":/resources/button.png"));//---------》需要图片素材
        painter->drawText(boundingRect(), Qt::AlignCenter, "FREEZE");
        painter->setPen(Qt::transparent);

        // 冷却中：绘制黑色遮罩
        if (counter < cool_time)
        {
            QBrush brush(QColor(0, 0, 0, 200));
            painter->setBrush(brush);
            painter->drawRect(QRectF(-78, -19, 156, 38 * (1 - qreal(counter) / cool_time)));
        }
    }
    // -------- 绘制 强化按钮 --------
    else if (type == POWER)
    {
        painter->drawPixmap(QRect(-80, -20, 160, 40), QPixmap(":/resources/button.png"));
        painter->drawText(boundingRect(), Qt::AlignCenter, "POWER");
        painter->setPen(Qt::transparent);

        // 冷却遮罩
        if (counter < cool_time)
        {
            QBrush brush(QColor(0, 0, 0, 200));
            painter->setBrush(brush);
            painter->drawRect(QRectF(-78, -19, 156, 36 * (1 - qreal(counter) / cool_time)));
        }
    }
    // -------- 绘制 铲子按钮 --------
    else if (type == SHOVEL)
    {
        painter->drawPixmap(QRect(-40, -40, 80, 80), QPixmap(":/resources/shovelbank.png"));
        painter->drawPixmap(QRect(-35, -35, 70, 70), QPixmap(":/resources/Shovel.png"));
        painter->setPen(Qt::transparent);

        // 冷却遮罩
        if (counter < cool_time)
        {
            QBrush brush(QColor(0, 0, 0, 200));
            painter->setBrush(brush);
            painter->drawRect(QRectF(-40, -38, 80, 76 * (1 - qreal(counter) / cool_time)));
        }
    }
}

// 鼠标点击事件：按钮核心功能
void Button::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (state) // 只有可点击时才生效
    {
        // -------- 点击 冰冻按钮 --------
        if (type == FREEZE)
        {
            // 取消其他模式
            if ((shovel_activate || power_activate) && event->button() == Qt::LeftButton)
            {
                shovel_activate = false;
                power_activate = false;
            }
            // 正常使用冰冻
            if (!shovel_activate && !power_activate && event->button() == Qt::LeftButton)
            {
                counter = 0;     // 重置冷却
                state = 0;       // 进入冷却

                // 遍历场景所有敌人，全部减速
                QList<QGraphicsItem*> items = scene()->items();
                if (!items.isEmpty())
                {
                    QGraphicsItem* item;
                    foreach (item, items)
                    {
                        // 把 Zombie 改为 Enemy
                        Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
                        if (enemy)
                        {
                            enemy->slow = SLOW;
                            enemy->counter = 0;
                        }
                    }
                }
            }
        }

        // -------- 点击 强化按钮 --------
        if (type == POWER)
        {
            if (shovel_activate && event->button() == Qt::LeftButton)
                shovel_activate = false;

            // 切换强化模式
            if (power_activate == false && event->button() == Qt::LeftButton)
            {
                power_activate = true;
                qDebug() << "power";
            }
            else if (power_activate == true && event->button() == Qt::LeftButton)
            {
                power_activate = false;
                qDebug() << "no_power";
            }
        }

        // -------- 点击 铲子按钮 --------
        if (type == SHOVEL)
        {
            if (power_activate && event->button() == Qt::LeftButton)
                power_activate = false;

            // 切换铲子模式
            if (shovel_activate == false && event->button() == Qt::LeftButton)
            {
                shovel_activate = true;
                qDebug() << "shovel_activate";
            }
            else if (shovel_activate == true && event->button() == Qt::LeftButton)
            {
                shovel_activate = false;
                qDebug() << "shovel_not_activate";
            }
        }
    }
}

// 返回按钮的矩形范围
QRectF Button::boundingRect() const
{
    if (type == SHOVEL)
        return QRectF(-42, -36, 80, 72); // 铲子按钮大小
    return QRectF(-80, -20, 160, 40);    // 功能按钮大小
}