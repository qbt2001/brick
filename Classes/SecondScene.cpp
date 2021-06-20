#include "SecondScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;
Scene* SecondScene::createScene() {
    auto scene = Scene::create();
    auto layer = SecondScene::create();
    scene->addChild(layer);
    return scene;
}
bool SecondScene::init() {
    Size size = Director::getInstance()->getVisibleSize();
    auto* background = Sprite::create("rank_back_ground.jpg");
    background->setPosition(size.width / 2, size.height / 2);
    background->setScale(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
    addChild(background, 0);
    int rank1 = UserDefault::getInstance()->getIntegerForKey("rank1");
    auto label1 = Label::createWithSystemFont("Rank1: " + std::to_string(rank1), "Arial", 40);
    label1->setPosition(Vec2(235, 230));
    this->addChild(label1);
    int rank2 = UserDefault::getInstance()->getIntegerForKey("rank2");
    auto label2 = Label::createWithSystemFont("Rank2: " + std::to_string(rank2), "Arial", 40);
    label2->setPosition(Vec2(235, 160));
    this->addChild(label2);
    int rank3 = UserDefault::getInstance()->getIntegerForKey("rank3");
    auto label3 = Label::createWithSystemFont("Rank3: " + std::to_string(rank3), "Arial", 40);
    label3->setPosition(Vec2(235, 90));
    this->addChild(label3);
    MenuItemImage *pCloseItem = MenuItemImage::create(
        "back1.png",
        "back1.png",
        CC_CALLBACK_1(SecondScene::popScene, this));
    pCloseItem->setScale(0.06);
    pCloseItem->setPosition(Point(Director::getInstance()->getWinSize().width - 20, 20));


    Menu* pMenu = Menu::create(pCloseItem, NULL);
    pMenu->setPosition(Point(0, 0));
    this->addChild(pMenu, 1);
    return true;
}

void SecondScene::popScene(Ref* pSender) {
    Director::getInstance()->popScene();
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, HelloWorld::createScene()));
}
