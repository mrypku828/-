#ifndef COINMAKER_H
#define COINMAKER_H

#include "paotai.h"

class CoinMaker : public Paotai
{
public:
    CoinMaker();
    void advance(int phase) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // COINMAKER_H