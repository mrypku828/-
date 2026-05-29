#include "map.h"
#include "paotai.h"
#include "CoinMaker.h"
#include "shooter.h"
#include "bomb.h"
#include "protecter.h"
#include "enemy.h"
#include "card.h"
#include "shop.h"
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QDebug>



//构造函数
//格子类型：1=可种植草地，0=边界/不可用
Map::Map(int gridType)
    : m_gridState(1)  // 1=可种植（阳光足够），0=阳光不足
    , m_gridType(gridType)
{
    //state=1 表示“钱够了”，state=0 表示“钱不够，鼠标变箭头”
}

//每帧更新逻辑
//图形项更新阶段（0=准备，1=执行）

void Map::advance(int phase)
{
    // 只对“可种植草地格子”执行更新
    if (m_gridType != 1)
        return;

    if (!phase)
        return;

    update();  // 触发重绘

    // 鼠标样式逻辑：选中卡片→手型；否则箭头
    if (Card::card_activate)
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        // 未选卡片 或 阳光不足 → 箭头
        setCursor(Qt::ArrowCursor);
    }
}

//绘制格子背景（当前为空，可后续加草地图片）
void Map::paint(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // 示例：绘制绿色边框（调试用）
    // painter->setPen(Qt::green);
    // painter->drawRect(boundingRect());
}


//鼠标左键点击：处理种植逻辑
//event 鼠标事件

void Map::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 仅可种植格子响应点击
    if (m_gridType != 1)
        return;

    // 1. 获取商店对象（场景固定位置）
    QGraphicsItem *shopItem = scene()->itemAt(QPoint(230, 0), transform());
    Shop *shop = qgraphicsitem_cast<Shop *>(shopItem);
    if (!shop)
        return;

    // 2. 获取当前选中卡片信息
    int cardId = Card::card_id;
    int plantCost = Card::cost[Card::mapping[cardId]];

    // 3. 左键 + 有选中卡片 + 金币足够 → 放置
    if (event->button() == Qt::LeftButton
        && Card::card_activate
        && shop->coin_val >= plantCost)
    {
        // 扣金币（防重复扣）
        if (shop->coin_val != shop->coin_val - plantCost)
        {
            shop->coin_val -= plantCost;
        }

        // 取消卡片选中状态
        QGraphicsItem *cardItem = scene()->itemAt(QPoint(Card::pos[cardId]), transform());
        Card *card = qgraphicsitem_cast<Card *>(cardItem);
        if (card)
        {
            card->state = 0;
            card->counter = 0;
        }
        Card::card_activate = false;

        // 计算放置位置（对齐网格）
        QPointF scenePos = mapToScene(event->pos());
        int plantX = ((int(scenePos.x()) - 249) / (GAP_X - 30)) * (GAP_X - 30) + PLANT_START;
        int plantY = ((int(scenePos.y()) - 81) / GAP_Y) * GAP_Y + ROW1;

        // 该位置已有炮台 → 不放置
        QGraphicsItem *itemAtPos = scene()->itemAt(QPoint(plantX, plantY), transform());
        if (itemAtPos && itemAtPos->type() == Paotai::Type)
        {
            return;
        }

        // 根据卡片ID创建对应植物
        Paotai *newPlant = nullptr;
        switch (cardId)
        {
        case 0: newPlant = new CoinMaker;    break;
        case 1: newPlant = new Shooter;  break;
        case 2: newPlant = new Protecter; break;
        case 3: newPlant = new Bomb;  break;

        default:
            qDebug() << "无效的炮台ID：" << cardId;
            return;
        }

        // 设置炮台位置并加入场景
        newPlant->setPos(plantX, plantY);
        newPlant->setZValue(2); // 层级在敌人之上
        scene()->addItem(newPlant);
    }
    // 金币不足 或 右键 → 取消选中
    else if (shop->coin_val < plantCost || event->button() == Qt::RightButton)
    {
        m_gridState = 0;          // 标记金币不足
        Card::card_activate = false; // 取消卡片选中
    }
}

//定义格子的点击/碰撞范围
//矩形区域

QRectF Map::boundingRect() const
{
    // 可种植格子：大区域；边界格子：全屏
    if (m_gridType == 1)
    {
        return QRectF(-369, -235, 738, 470);
    }
    else
    {
        return QRectF(0, 0, WIDTH, HEIGHT);
    }
}