#ifndef PROTECTER_H
#define PROTECTER_H

#include "paotai.h"


// 防御炮台：高血量，专门阻挡敌人前进
class Protecter : public Paotai
{
public:
    Protecter();    // 构造函数
    void advance(int phase) override;                // 每帧更新
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override; // 鼠标点击（铲除外）
};

#endif // PROTECTER_H