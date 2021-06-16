#ifndef __GameOver_H__
#define __GameOver_H__
#include "cocos2d.h"
using namespace cocos2d;
class GameOver : public Layer {
public:
    static Scene* createScene();
    virtual bool init();
    void CallBack(cocos2d::Ref* pSender);
    void Exit(cocos2d::Ref* pSender);
    CREATE_FUNC(GameOver);

    void popScene(Ref* pSender);
};
#endif