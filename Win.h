#ifndef __Win_H__
#define __Win_H__
#include "cocos2d.h"
using namespace cocos2d;
class Win : public Layer {
public:
    static Scene* createScene();
    virtual bool init();
    void Exit(cocos2d::Ref* pSender);
    CREATE_FUNC(Win);

};
#endif