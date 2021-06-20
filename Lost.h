#ifndef __Lost_H__
#define __Lost_H__
#include "cocos2d.h"
using namespace cocos2d;
class Lost : public Layer {
public:
    static Scene* createScene();
    virtual bool init();
    void Exit(cocos2d::Ref* pSender);
    CREATE_FUNC(Lost);

};
#endif