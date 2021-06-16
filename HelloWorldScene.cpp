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
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    //auto closeItem = MenuItemImage::create(
    //                                       "CloseNormal.png",
    //                                       "CloseSelected.png",
    //                                       CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    //if (closeItem == nullptr ||
    //    closeItem->getContentSize().width <= 0 ||
    //    closeItem->getContentSize().height <= 0)
    //{
    //    problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    //}
    //else
    //{
    //    float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
    //    float y = origin.y + closeItem->getContentSize().height/2;
    //    closeItem->setPosition(Vec2(x,y));
    //}

    //// create menu, it's an autorelease object
    //auto menu = Menu::create(closeItem, NULL);
    //menu->setPosition(Vec2::ZERO);
    //this->addChild(menu, 1);

    auto pbutton1 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(HelloWorld::gameover, this));
    Size size = Director::getInstance()->getVisibleSize();
    auto label1= Label::createWithSystemFont("Start", "Arial", 280);
    auto plabel1 = MenuItemLabel::create(
        label1,
        CC_CALLBACK_1(HelloWorld::gameover, this));
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

    //UserDefault::getInstance()->setStringForKey("string", "0 0 0 ");
    HelloWorld::write(2);/*
    HelloWorld::write(5);
    HelloWorld::write(1);*/
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
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    //if (label == nullptr)
    //{
    //    problemLoading("'fonts/Marker Felt.ttf'");
    //}
    //else
    //{
    //    // position the label on the center of the screen
    //    label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                            origin.y + visibleSize.height - label->getContentSize().height));

    //    // add the label as a child to this layer
    //    this->addChild(label, 1);
    //}

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //}
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

    /*std::string str = UserDefault::getInstance()->getStringForKey("string");
    int number = 0;
    int k = 0, rank_array[3] = { 0 };
    for (int i = 0; i < str.length(); i++)
        if (str[i] >= '0' && str[i] <= '9')
            number = number * 10 + str[i] - '0';
        else
        {
            rank_array[k] = number;
            number = 0;
            k++;
            if (k == 3) break;
        }
    if (insert_number > rank_array[0])
        rank_array[2] = rank_array[1], rank_array[1] = rank_array[0], rank_array[0] = insert_number;
    else if (insert_number > rank_array[1])
        rank_array[2] = rank_array[1], rank_array[1] = insert_number;
    else if (insert_number > rank_array[2])
        rank_array[2] = insert_number;
    std::string result = std::to_string(rank_array[0]) + ' ' + std::to_string(rank_array[1]) + ' ' + std::to_string(rank_array[2]) + ' ';
    UserDefault::getInstance()->setStringForKey("string", result); */
}
void HelloWorld::read(Ref* pSender)
{
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, SecondScene::createScene()));
}

void HelloWorld::menuClose(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->pushScene(TransitionFade::create(1.0f,SecondScene::createScene()));

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::gameover(Ref* pSender)
{
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, GameOver::createScene()));
}

void HelloWorld::TurnToMyScene(Ref* pSender)
{
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, MyWorld::createScene()));
}

