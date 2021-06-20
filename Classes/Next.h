#ifndef __Next_H__
#define __Next_H__
#include "cocos2d.h"
using namespace cocos2d;
class Next : public Layer {
public:
    static Scene* createScene();
    virtual bool init();
    void CallBack(cocos2d::Ref* pSender);
    void Exit(cocos2d::Ref* pSender);
    CREATE_FUNC(Next);

    void popScene(Ref* pSender);
};
#endif