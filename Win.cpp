#include "Win.h"
#include "HelloWorldScene.h"
USING_NS_CC;
Scene* Win::createScene() {
    auto scene = Scene::create();
    auto layer = Win::create();
    scene->addChild(layer);
    return scene;
}
bool Win::init() {
    Size size = Director::getInstance()->getVisibleSize();
    auto* background = Sprite::create("rank_back_ground.jpg");
    background->setPosition(size.width / 2, size.height / 2);
    background->setScale(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
    addChild(background, 0);
    
    auto label1 = Label::createWithSystemFont("Win! ", "Arial", 40);
    label1->setPosition(size.width / 2, size.height / 6 * 5);
    this->addChild(label1);

    auto pbutton2 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(Win::Exit, this));
    auto label2 = Label::createWithSystemFont("Exit", "Arial", 280);
    auto plabel2 = MenuItemLabel::create(
        label2,
        CC_CALLBACK_1(Win::Exit, this));
    auto button2 = Menu::create(pbutton2, plabel2, NULL);
    button2->setScale(size.width / button2->getContentSize().width / 20, size.height / button2->getContentSize().height / 20);
    button2->setPosition(Vec2(3, -50));
    this->addChild(button2, 2);
    return true;
}


void Win::Exit(Ref* pSender) {
    Director::getInstance()->end();
}