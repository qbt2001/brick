#include "MyScene.h"
#include"GameOver.h"
#include "ui/CocosGUI.h"     //ui头文件
#include"Next.h"
#include<cmath>
#define pi 3.14
using namespace cocos2d::ui; //ui命名空间
USING_NS_CC;

/*获取关卡
levelTag为true则关卡加一
levelTag为false则关卡回到第一关*/
int getChapter(bool levelTag)
{
	static int GameLevel = 0;
	if (levelTag == true)
		GameLevel++;
	else
		GameLevel=0;
	return GameLevel;
}

/*获取总得分*/
int getTotalScore(int tag,bool tag2)
{
	static int score=0;
	score += 1 + tag;
	if (true == tag2)
	{
		score = 0;
	}
	return score;
}

Scene* MyWorld::createScene()
{
	auto scene = Scene::createWithPhysics();   //创建一个场景对象
	auto layer = MyWorld::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f));
	scene->addChild(layer);                   //将layer加入到场景scene中去
	return scene;
}
bool MyWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}
	ScreenSize = Director::getInstance()->getVisibleSize();
	//ScreenSize.width /= 2;

	gameLevel = getChapter(true);
	MyWorld::updateChapter();
	MyWorld::addSprite();
	MyWorld::addListener();
	MyWorld::addScoreLine();
	MyWorld::makeBricks();

	Node::schedule(CC_SCHEDULE_SELECTOR(MyWorld::update), 0.02f, kRepeatForever, 0.1f);

	return true;
}


/*添加背景、地板、移动板及小球精灵*/
void MyWorld::addSprite()
{
	/*设置边界*/
	auto edgeBody = PhysicsBody::createEdgeBox(ScreenSize, PhysicsMaterial(100.0f, 1.0f, 1.0f), 0.1f);     //三参数代表矩形边界的大小、材质(密度、恢复系数、摩擦力)、边框宽度
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(ScreenSize.width / 2, ScreenSize.height / 2));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode, 0);

	/*添加背景*/
	auto* backGround = Sprite::create("bg.jpg");
	backGround->setPosition(ScreenSize.width / 2, ScreenSize.height / 2);
	backGround->setScale(ScreenSize.width / backGround->getContentSize().width, ScreenSize.height / backGround->getContentSize().height);
	this->addChild(backGround);                   //参数0表示drawing优先级

	/*添加地板*/
	myFloor = Sprite::create("brick.png");
	myFloor->setPosition(ScreenSize.width / 2, 0);
	myFloor->setScale(ScreenSize.width / myFloor->getContentSize().width, 0.1f);
	auto floorBody = PhysicsBody::createBox(myFloor->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	floorBody->setContactTestBitmask(0xFFFFFFFF);
	floorBody->setRotationEnable(false);   //不旋转
	floorBody->setTag(Tag::FLOOR);         //判断节点
	floorBody->setDynamic(false);
	
	myFloor->setPhysicsBody(floorBody);
	this->addChild(myFloor);

	/*添加底板*/
	board = Sprite::create("board.png");
	board->setScale(boardLength, 0.08f);
	board->setPosition(ScreenSize.width / 2, myFloor->getContentSize().height*0.1f);
	auto boardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
	boardBody->setContactTestBitmask(0xFFFFFFFF);
	boardBody->setRotationEnable(false);   //不旋转
	boardBody->setTag(Tag::BOARD);         //判断节点
	boardBody->setDynamic(false);
	board->setPhysicsBody(boardBody);
	this->addChild(board);              //创建底板并加入到场景中

	/*添加小球*/
	ball = Sprite::create("ball.png");
	ball->setScale(0.1f);
	ball->setPosition(ScreenSize.width / 2, board->getContentSize().height * 0.08f + ball->getContentSize().width / 20 + myFloor->getContentSize().height / 10);
	ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	ballBody->setContactTestBitmask(0xFFFFFFFF);
	ballBody->setGravityEnable(false);
	ballBody->setRotationEnable(false);   //不旋转
	ballBody->setTag(Tag::BALL);          //判别节点
	ballBody->setDynamic(true);
	ballBody->setVelocity(Vec2(originalVelocity-15.0f+getTotalScore(-1,0)*2, originalVelocity+getTotalScore(-1,0)*2));//设置初始速度

	ball->setPhysicsBody(ballBody);
	this->addChild(ball);             //创建小球并加入到场景中

}

/*添加实时变化的得分栏
借鉴代码出处：https://blog.csdn.net/Tang_AHMET/article/details/105145519
*/
void MyWorld::addScoreLine()
{
	auto scoreLabel = Label::createWithTTF("Total score: ","fonts/Marker Felt.ttf",15);
	scoreText = TextAtlas::create(std::to_string(totalScore), "score.png", 19, 19, "0");

	scoreLabel->setPosition(ScreenSize.width * 0.1f, ScreenSize.height * 0.2f);
	scoreText->setPosition(Vec2(ScreenSize.width * 0.2f, ScreenSize.height * 0.2f));

	this->addChild(scoreLabel);
	this->addChild(scoreText);

	scoreText->setString(std::to_string(getTotalScore(-1,0)));
}

/*设定砖块类型*/
Sprite* MyWorld::setBrickType(int brickType)
{
	if (Tag::SCORE_PROP == brickType)
	{
		brick = Sprite::create("scoreProp.png");
	}
	else if (Tag::BOARD_PROP == brickType)
	{
		brick = Sprite::create("boardProp.png");
	}
	else
	{
		brick = Sprite::create("brick.png");
	}
	brick->setScale(0.2f, 0.2f);

	return brick;
}

/*生成单个砖块
参数：位置坐标X,位置坐标Y
砖块类型*/
void MyWorld::makeSingleBrick(float& PositionX,float& PositionY,int brickType)
{
	brick->setPosition(PositionX, PositionY);

	auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	brickBody->setContactTestBitmask(0xFFFFFFFF);
	brickBody->setRotationEnable(false);   //不旋转
	brickBody->setTag(brickType);          //判断节点
	brickBody->setDynamic(false);

	brick->setPhysicsBody(brickBody);
	this->addChild(brick, 2);

}

/*生成不同关卡的砖块*/
void MyWorld::makeBricks()
{
	float positionX = 0;
	float positionY = 0;
	if (1 == gameLevel)
	{
		for (unsigned int i = 1; i <= 2; i++)
		{
			MyWorld::setBrickType(Tag::BRICK);
			positionX = ScreenSize.width/2-brick->getContentSize().width/5*3;
			for(unsigned int j=1;j<=7;j++)
			{
				MyWorld::setBrickType(Tag::BRICK);
				positionY = ScreenSize.height - brick->getContentSize().height / 5 * i;
				MyWorld::makeSingleBrick(positionX, positionY,Tag::BRICK);
				positionX = positionX + brick->getContentSize().width / 5;
			}
		}
		MyWorld::setBrickType(Tag::BOARD_PROP);
		positionX = ScreenSize.width / 2 + brick->getContentSize().width / 5 * 3;
		positionY = ScreenSize.height - brick->getContentSize().height / 5 * 3;
		MyWorld::makeSingleBrick(positionX, positionY, Tag::BOARD_PROP);

		MyWorld::setBrickType(Tag::SCORE_PROP);
		positionX = ScreenSize.width / 2 - brick->getContentSize().width / 5 * 3;
		MyWorld::makeSingleBrick(positionX, positionY, Tag::SCORE_PROP);
	}
	else if (2 == gameLevel)
	{
		for (unsigned int i = 1; i <= 5; i += 2)
		{
			MyWorld::setBrickType(Tag::BRICK);
			positionX = ScreenSize.width / 2 - brick->getContentSize().width / 10 * (i - 1);
			for (unsigned int j = 1; j <= i; j++)
			{
				MyWorld::setBrickType(Tag::BRICK);
				positionY =ScreenSize.height- brick->getContentSize().height / 5 * (i + 1) / 2;
				MyWorld::makeSingleBrick(positionX, positionY,Tag::BRICK);
				positionX = positionX + brick->getContentSize().width / 5;
			}
		}
		MyWorld::setBrickType(Tag::BRICK);
		positionX = ScreenSize.width / 2 - brick->getContentSize().width / 5*3;
		positionY = ScreenSize.height - brick->getContentSize().height / 5 * 4;
		MyWorld::makeSingleBrick(positionX, positionY, Tag::BRICK);

		MyWorld::setBrickType(Tag::BRICK);
		positionX = ScreenSize.width / 2 + brick->getContentSize().width / 5 * 3;
		positionY = ScreenSize.height - brick->getContentSize().height / 5 * 4;
		MyWorld::makeSingleBrick(positionX, positionY, Tag::BRICK);

		/*创建道具*/
		MyWorld::setBrickType(Tag::BOARD_PROP);
		positionX = ScreenSize.width / 2 - brick->getContentSize().width / 10;
		positionY = ScreenSize.height-brick->getContentSize().height / 5 * 4;
		MyWorld::makeSingleBrick(positionX, positionY,Tag::BOARD_PROP);
		MyWorld::setBrickType(Tag::SCORE_PROP);
		positionX = ScreenSize.width / 2 + brick->getContentSize().width / 10;
		MyWorld::makeSingleBrick(positionX, positionY,Tag::SCORE_PROP);

		for (unsigned int i = 1; i <= 5; i += 2)
		{
			MyWorld::setBrickType(Tag::BRICK);
			positionX = ScreenSize.width / 2 - brick->getContentSize().width / 10 * (5-i);
			for (unsigned int j = 1;j<=6-i ; j++)
			{
				MyWorld::setBrickType(Tag::BRICK);
				positionY = ScreenSize.height - brick->getContentSize().height / 5 * ((i + 1) / 2 + 4);
				makeSingleBrick(positionX, positionY, Tag::BRICK);
				positionX = positionX + brick->getContentSize().width / 5;
			}
		}
	}
}

/*添加监听器
借鉴代码出处：https://blog.csdn.net/yaoxh6/article/details/80765819?utm_source=app&app_version=4.8.0&code=app_1562916241&uLinkId=usr1mkqgl919blen*/
void MyWorld::addListener()
{
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MyWorld::KeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MyWorld::KeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//add contact event listerner
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MyWorld::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

/*按下一个键，执行对应操作*/
void MyWorld::KeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		if (board->getPosition().x > board->getContentSize().width*boardLength/2)
		{
			board->getPhysicsBody()->setVelocity(Vec2(-400, 0));
		}
		else
		{
			board->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case EventKeyboard::KeyCode::KEY_D:
		if (board->getPosition().x < ScreenSize.width - board->getContentSize().width*boardLength / 2)
		{
			board->getPhysicsBody()->setVelocity(Vec2(400, 0));
		}
		else
		{
			board->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	default:break;
	}
}

/*松开按键，执行对应操作*/
void MyWorld::KeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_D:
		board->getPhysicsBody()->setVelocity(Vec2(0, 0));
		break;
	default:break;
	}
}

/*碰撞检测函数*/
bool MyWorld::onContactBegin(PhysicsContact& contact)
{
	
	auto bodyA = contact.getShapeA()->getBody();
	auto nodeA = bodyA->getNode();
	auto bodyB = contact.getShapeB()->getBody();
	auto nodeB = bodyB->getNode();

	if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::FLOOR)        //判断游戏结束
	{
		MyWorld::write(totalScore);
		getChapter(false);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOver::createScene()));
	}
	else if (bodyA->getTag() == Tag::FLOOR && bodyB->getTag() == Tag::BALL)
	{
		MyWorld::write(totalScore);
		getChapter(false);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOver::createScene()));
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BRICK)        //砖块碰撞
	{
		nodeB->removeFromParentAndCleanup(true);
		amountOfBrick--;
		totalScore=getTotalScore(scoreTag,0);
		scoreText->setString(std::to_string(totalScore));

	}
	else if (bodyA->getTag() == Tag::BRICK && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		amountOfBrick--;
		totalScore=getTotalScore(scoreTag,0);
		scoreText->setString(std::to_string(totalScore));
	}
	else if (bodyA->getTag() == Tag::SCORE_PROP && bodyB->getTag() == Tag::BALL)//得分道具
	{
		nodeA->removeFromParentAndCleanup(true);
		scoreTag = 1;
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::SCORE_PROP)
	{
		nodeB->removeFromParentAndCleanup(true);
		scoreTag = 1;
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BOARD_PROP)//底板增长道具
	{
		nodeB->removeFromParentAndCleanup(true);
		boardLength *= 1.4f;
		board->setScale(boardLength, 0.1f);
	}
	else if (bodyA->getTag() == Tag::BOARD_PROP && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		boardLength *= 1.4f;
		board->setScale(boardLength, 0.1f);
	}

	if (0 == amountOfBrick && 1 == gameLevel)				//通过第一关
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Next::createScene()));
	}
	else if (0 == amountOfBrick && 2 == gameLevel)          //通过第二关
	{
		MyWorld::write(totalScore);
		getTotalScore(scoreTag, 1);
		getChapter(false);
	    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOver::createScene()));
	}

	return true;
}
void MyWorld::update(float dt)     
{
	/*更新板的状态*/
	if (board->getPosition().x < board->getContentSize().width* boardLength/2)
	{
		board->setPosition(Vec2(board->getContentSize().width * boardLength / 2, board->getPosition().y));
	}
	else if (board->getPosition().x > ScreenSize.width - board->getContentSize().width*boardLength/2)
	{
		board->setPosition(ScreenSize.width - board->getContentSize().width * boardLength / 2, board->getPosition().y);
	}

	/*更新小球速度*/
	float Vx = ballBody->getVelocity().x;
	float Vy = ballBody->getVelocity().y;
	float angle = atan(fabs(Vy / Vx));
	float V = float(sqrt(2 * originalVelocity * originalVelocity) + totalScore* 2);
	ballBody->setVelocity(Vec2(Vx > 0 ? V * cos(angle) : -V * cos(angle), Vy > 0 ? V * sin(angle) : -V * sin(angle)));
}

/*更新关卡数据*/
void MyWorld::updateChapter()
{
	scoreTag = 0;
	switch (gameLevel)
	{
	case 1:
		amountOfBrick = 14;
		boardLength = 0.1f;
		originalVelocity = 100.0f;
		break;
	case 2:
		amountOfBrick = 20;
		boardLength = 0.08f;
		originalVelocity = 105.0f;
		break;
	default: break;
	}
}

void MyWorld::write(int insert_number)
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