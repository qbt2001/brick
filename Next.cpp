#include "Next.h"
#include "HelloWorldScene.h"
#include "MyScene.h"
USING_NS_CC;
Scene* Next::createScene() {
    auto scene = Scene::create();
    auto layer = Next::create();
    scene->addChild(layer);
    return scene;
}
bool Next::init() {
    Size size = Director::getInstance()->getVisibleSize();
    auto* background = Sprite::create("rank_back_ground.jpg");
    background->setPosition(size.width / 2, size.height / 2);
    background->setScale(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
    addChild(background, 0);

    auto pbutton1 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(Next::CallBack, this));
    auto label1 = Label::createWithSystemFont("Next", "Arial", 280);
    auto plabel1 = MenuItemLabel::create(
        label1,
        CC_CALLBACK_1(Next::CallBack, this));
    auto button1 = Menu::create(pbutton1, plabel1, NULL);
    button1->setScale(size.width / button1->getContentSize().width / 20, size.height / button1->getContentSize().height / 20);
    button1->setPosition(Vec2(3, 0));
    this->addChild(button1, 2);

    auto pbutton2 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(Next::Exit, this));
    auto label2 = Label::createWithSystemFont("Exit", "Arial", 280);
    auto plabel2 = MenuItemLabel::create(
        label2,
        CC_CALLBACK_1(Next::Exit, this));
    auto button2 = Menu::create(pbutton2, plabel2, NULL);
    button2->setScale(size.width / button2->getContentSize().width / 20, size.height / button2->getContentSize().height / 20);
    button2->setPosition(Vec2(3, -50));
    this->addChild(button2, 2);
    return true;
}

void Next::CallBack(Ref* pSender) {
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MyWorld::createScene()));
}

void Next::Exit(Ref* pSender) {
    Director::getInstance()->end();
}