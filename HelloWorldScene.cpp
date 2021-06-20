/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "../win32-build/SecondScene.h"
#include "../win32-build/GameOver.h"
#include "../win32-build/MyScene.h"
#include "../win32-build/Next.h"
#include "../win32-build/OnlineScene.h"
#include "../win32-build/Music.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    PlayBackgroundMusic();

    auto pbutton1 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(HelloWorld::TurnToOnlineScene, this));
    Size size = Director::getInstance()->getVisibleSize();
    auto label1= Label::createWithSystemFont("Online Mode", "Arial", 280);
    auto plabel1 = MenuItemLabel::create(
        label1,
        CC_CALLBACK_1(HelloWorld::TurnToOnlineScene, this));
    auto button1 = Menu::create(pbutton1, plabel1, NULL);
    button1->setScale(size.width / button1->getContentSize().width / 20, size.height / button1->getContentSize().height / 20);
    button1->setPosition(Vec2(3,50));
    this->addChild(button1, 2);

    auto pbutton2 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(HelloWorld::menuClose, this));
    auto label2 = Label::createWithSystemFont("Exit", "Arial", 280);
    auto plabel2 = MenuItemLabel::create(
        label2,
        CC_CALLBACK_1(HelloWorld::menuClose, this));
    auto button2 = Menu::create(pbutton2, plabel2, NULL);
    button2->setScale(size.width / button2->getContentSize().width / 20, size.height / button2->getContentSize().height / 20);
    button2->setPosition(Vec2(3, -50));
    this->addChild(button2, 2);

    auto pbutton3 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(HelloWorld::read, this));
    auto label3 = Label::createWithSystemFont("Rank", "Arial", 280);
    auto plabel3 = MenuItemLabel::create(
        label3,
        CC_CALLBACK_1(HelloWorld::read, this));
    auto button3 = Menu::create(pbutton3, plabel3, NULL);
    button3->setScale(size.width / button3->getContentSize().width / 20, size.height / button3->getContentSize().height / 20);
    button3->setPosition(Vec2(3, 0));
    this->addChild(button3, 3);

    auto pbutton4 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(HelloWorld::TurnToMyScene, this));
    auto label4 = Label::createWithSystemFont("Single Mode", "Arial", 280);
    auto plabel4 = MenuItemLabel::create(
        label4,
        CC_CALLBACK_1(HelloWorld::TurnToMyScene, this));
    auto button4 = Menu::create(pbutton4, plabel4, NULL);
    button4->setScale(size.width / button4->getContentSize().width / 20, size.height / button4->getContentSize().height / 20);
    button4->setPosition(Vec2(3, 100));
    this->addChild(button4, 2);

    auto* background = Sprite::create("rank_back_ground.jpg");
    background->setPosition(size.width / 2, size.height / 2);
    background->setScale(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
    addChild(background, 0);
    return true;
}


void HelloWorld::write(int insert_number)
{
    int rank1 = UserDefault::getInstance()->getIntegerForKey("rank1");
    int rank2 = UserDefault::getInstance()->getIntegerForKey("rank2");
    int rank3 = UserDefault::getInstance()->getIntegerForKey("rank3");
    if (insert_number > rank1)
        rank3 = rank2, rank2 = rank1, rank1 = insert_number;
    else if (insert_number > rank2)
        rank3 = rank2, rank2 = insert_number;
    else if (insert_number > rank3)
        rank3 = insert_number;
    UserDefault::getInstance()->setIntegerForKey("rank1", rank1);
    UserDefault::getInstance()->setIntegerForKey("rank2", rank2);
    UserDefault::getInstance()->setIntegerForKey("rank3", rank3);
}
void HelloWorld::read(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SecondScene::createScene()));
}

void HelloWorld::menuClose(Ref* pSender)
{
    PauseBackgroundMusic();
    Director::getInstance()->end();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f,SecondScene::createScene()));
}

void HelloWorld::gameover(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOver::createScene()));
}

void HelloWorld::TurnToMyScene(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MyWorld::createScene()));
}

void HelloWorld::TurnToNext(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Next::createScene()));
}

void HelloWorld::TurnToOnlineScene(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, OnlineScene::createScene()));
}
