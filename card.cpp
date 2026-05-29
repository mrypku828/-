#include "card.h"
#include "button.h"
#include <QDebug>

// ===================== 全局卡片静态数据 =====================
//名称 -> 卡片ID
const QMap<QString, int> Card::id = {
    {"coinmaker", 0},
    {"shooter", 1},
    {"protecter", 2},
    {"bomb", 3}
};

// 卡片ID -> 界面显示坐标
const QMap<int, QPoint> Card::pos = {
    {0, QPoint(350, 45)},  // 第一格
    {1, QPoint(420, 45)},  // 第二格
    {2, QPoint(490, 45)},  // 第三格
    {3, QPoint(560, 45)}   // 第四格
};

// 卡片ID -> 名称 (与id映射表相反)
const QMap<int, QString> Card::mapping = {
    {0, "coinmaker"},
    {1, "shooter"},
    {2, "protecter"},
    {3, "bomb"}
};

// 名称 -> 冷却时间 (设定了默认值，可根据游戏平衡调整)
const QMap<QString, int> Card::cool = {
    {"coinmaker", 150},    // 冷却快
    {"shooter", 200},       // 中等
    {"protecter", 300},     // 冷却慢
    {"bomb", 450}           // 冷却最慢
};

// 名称 -> 消耗阳光 (可根据游戏平衡调整)
const QMap<QString, int> Card::cost = {
    {"coinmaker", 50},
    {"shooter", 100},
    {"protecter", 50},
    {"bomb", 250}
};

// ===================== 全局静态变量初始化 =====================
int Card::card_id = -1;             // 当前选中的卡片ID，-1表示未选中
bool Card::card_activate = false;    // 卡片是否处于激活状态（准备种植）

// ===================== 构造函数  =====================
Card::Card(const QString& name_)
{
    name = name_;        // 卡片对应的名称
    counter = 0;        // 冷却计时器初始化
    state = 0;          // 初始状态：冷却中
}

// ===================== 每帧更新：冷却倒计时 =====================
void Card::advance(int phase)
{
    if (!phase) return;
    update(); // 刷新画面

    // 冷却计时未完成
    if (counter < cool[name])
    {
        counter++;              // 计时增加
        state = 0;              // 状态：不可用
        setCursor(Qt::ArrowCursor);
    }
    // 冷却完成
    else
    {
        state = 1;              // 状态：可用
        setCursor(Qt::PointingHandCursor);
    }
}

// ===================== 绘制卡片：背景、图标、阳光、冷却遮罩 =====================
void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->scale(0.6, 0.58); // 缩放卡片显示

    // 绘制卡片背景
    painter->drawPixmap(QRect(-50, -70, 100, 140), QPixmap(":/resources/card.png"));
    // 绘制卡片图标 (注意：这里会根据name变量去加载图片)
    // 请确保你的资源文件中，这四种植物的图片文件名分别是：
    // coinmaker.png, shooter.png, protecter.png, bomb.png
    painter->drawPixmap(QRect(-35, -42, 70, 70), QPixmap(":/resources/" + name + ".png"));

    // 绘制阳光消耗文字
    QFont font;
    font.setPointSizeF(18);
    painter->setFont(font);
    painter->drawText(QPoint(-30, 62), QString::number(cost[name]));

    // 冷却中：绘制黑色遮罩
    if (counter < cool[name])
    {
        QBrush brush(QColor(0, 0, 0, 200));
        painter->setBrush(brush);
        painter->drawRect(QRectF(-48, -68, 98, 132 * (1 - qreal(counter) / cool[name])));
    }
}

// ===================== 卡片点击范围 (不变) =====================
QRectF Card::boundingRect() const
{
    return QRectF(-20, -70, 40, 120);
}

// ===================== 鼠标点击：选中/取消卡片 (不变) =====================
void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 左键点击：取消强化/铲子模式
    if ((Button::power_activate || Button::shovel_activate) && event->button() == Qt::LeftButton)
    {
        Button::power_activate = false;
        Button::shovel_activate = false;
    }
    // 冷却完成 + 未激活卡片：点击激活，准备种植
    else if (state && !card_activate && event->button() == Qt::LeftButton)
    {
        card_activate = true;
        Card::card_id = id[name];
        qDebug() << "卡片激活: " << name;
        qDebug() << "卡片ID: " << card_id;
    }
    // 已激活卡片：再次点击取消
    else if (state && card_activate && event->button() == Qt::LeftButton)
    {
        card_activate = false;
        qDebug() << "卡片取消激活";
    }
}