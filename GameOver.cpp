#include "GameOver.h"
#include "HelloWorldScene.h"
USING_NS_CC;
Scene* GameOver::createScene() {
    auto scene = Scene::create();
    auto layer = GameOver::create();
    scene->addChild(layer);
    return scene;
}
bool GameOver::init() {
    Size size = Director::getInstance()->getVisibleSize();
    auto* background = Sprite::create("rank_back_ground.jpg");
    background->setPosition(size.width / 2, size.height / 2);
    background->setScale(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
    addChild(background, 0);
    auto* gameover = Sprite::create("gameover.png");
    gameover->setPosition(size.width / 2, size.height / 6*5);
    gameover->setScale(size.width / gameover->getContentSize().width/3, size.height / gameover->getContentSize().height/7);
    addChild(gameover, 0);

    auto pbutton1 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(GameOver::CallBack, this));
    auto label1 = Label::createWithSystemFont("Replay", "Arial", 280);
    auto plabel1 = MenuItemLabel::create(
        label1,
        CC_CALLBACK_1(GameOver::CallBack, this));
    auto button1 = Menu::create(pbutton1, plabel1, NULL);
    button1->setScale(size.width / button1->getContentSize().width / 20, size.height / button1->getContentSize().height / 20);
    button1->setPosition(Vec2(3, 0));
    this->addChild(button1, 2);

    auto pbutton2 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(GameOver::Exit, this));
    auto label2 = Label::createWithSystemFont("Exit", "Arial", 280);
    auto plabel2 = MenuItemLabel::create(
        label2,
        CC_CALLBACK_1(GameOver::Exit, this));
    auto button2 = Menu::create(pbutton2, plabel2, NULL);
    button2->setScale(size.width / button2->getContentSize().width / 20, size.height / button2->getContentSize().height / 20);
    button2->setPosition(Vec2(3, -50));
    this->addChild(button2, 2);
    /*auto pbutton1 = MenuItemImage::create(
        "button.png",
        "button.png",
        CC_CALLBACK_1(Gameover::CallBack, this));
    Size size = Director::getInstance()->getVisibleSize();
    auto label1 = Label::createWithSystemFont("Start", "Arial", 280);
    auto plabel1 = MenuItemLabel::create(
        label1,
        CC_CALLBACK_1(Gameover::CallBack, this));
    auto button1 = Menu::create(pbutton1, plabel1, NULL);
    button1->setScale(size.width / button1->getContentSize().width / 20, size.height / button1->getContentSize().height / 20);
    button1->setPosition(Vec2(3, 50));
    this->addChild(button1, 2);*/
    return true;
}

void GameOver::CallBack(Ref* pSender) {
    Director::getInstance()->popScene();
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, HelloWorld::createScene()));
}

void GameOver::Exit(Ref* pSender) {
    Director::getInstance()->end();
}