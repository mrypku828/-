#ifndef BOMB_H
#define BOMB_H

#include "paotai.h"



// 爆炸范围相关偏移常量
#define GAP_Y 100
#define GAP_X 110

// 樱桃炸弹 → 炸弹类：一次性大范围高伤植物
class Bomb : public Paotai
{
public:
    Bomb();
    void advance(int phase) override;
    bool collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    QRectF boundingRect() const override;
};

#endif // BOMB_H