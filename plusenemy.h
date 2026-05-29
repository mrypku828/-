#ifndef PLUSENEMY_H
#define PLUSENEMY_H


#include "enemy.h"
#include "paotai.h"
#include "button.h"


class PlusEnemy : public Enemy
{
public:

    PlusEnemy();
    void advance(int phase) override;
    void snowfree() override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // PLUSENEMY_H