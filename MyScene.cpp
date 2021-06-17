#include "MyScene.h"
#include<cmath>
#define pi 3.14159

USING_NS_CC;
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

	/*设置边界*/
	auto edgeBody = PhysicsBody::createEdgeBox(ScreenSize, PhysicsMaterial(100.0f,1.0f,1.0f),0.1f);     //三参数代表矩形边界的大小、材质(密度、恢复系数、摩擦力)、边框宽度
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(ScreenSize.width / 2, ScreenSize.height / 2));
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);

	MyWorld::updateChapter();
	MyWorld::addSprite();
	MyWorld::addListener();
	MyWorld::test();
	MyWorld::makeBricks();


	Node::schedule(CC_SCHEDULE_SELECTOR(MyWorld::update), 0.01f, kRepeatForever, 0.1f);

	return true;
}

void MyWorld::addSprite()
{
	/*添加背景*/
	auto* backGround = Sprite::create("bg.jpg");
	backGround->setPosition(ScreenSize.width / 2, ScreenSize.height / 2);
	backGround->setScale(ScreenSize.width / backGround->getContentSize().width, ScreenSize.height / backGround->getContentSize().height);
	addChild(backGround,0);                   //参数0表示drawing优先级

	/*添加底板*/
	board = Sprite::create("board.png");
	board->setScale(boardLength, 0.1f);
	board->setPosition(ScreenSize.width / 2, 0);
	auto boardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
	boardBody->setContactTestBitmask(0xFFFFFFFF);
	boardBody->setRotationEnable(false);   //不旋转
	boardBody->setTag(Tag::BOARD);         //判断节点
	boardBody->setDynamic(false);
	board->setPhysicsBody(boardBody);
	this->addChild(board, 1);              //创建底板并加入到场景中

	/*添加地板*/
	myFloor= Sprite::create("floor.png");
	myFloor->setPosition(ScreenSize.width / 2, 0);
	myFloor->setScale(ScreenSize.width / myFloor->getContentSize().width, myFloor->getContentSize().height);
	auto floorBody = PhysicsBody::createBox(myFloor->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	floorBody->setRotationEnable(false);   //不旋转
	floorBody->setTag(Tag::FLOOR);         //判断节点
	floorBody->setDynamic(false);

	myFloor->setPhysicsBody(floorBody);
	this->addChild(myFloor);

}

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
		brick = Sprite::create("brick2.png");
	}
	brick->setScale(0.2f, 0.2f);

	return brick;
}

/*生成单个砖块
参数：位置坐标X,位置坐标Y
砖块类型*/
void MyWorld::makeSingleBrick(float& PositionX,float& PositionY,int brickType)
{
	brick->setPosition(PositionX,PositionY);

	auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	brickBody->setContactTestBitmask(0xFFFFFFFF);
	brickBody->setRotationEnable(false);   //不旋转
	brickBody->setTag(brickType);         //判断节点
	brickBody->setDynamic(false);

	brick->setPhysicsBody(brickBody);
	this->addChild(brick, 2);
}

void MyWorld::makeBricks()
{
	float positionX = 0;
	float positionY = 0;
	if (1 == gameLevel)
	{
		for (unsigned int i = 1; i <= 3; i++)
		{
			positionX = 15;
			while (positionX < ScreenSize.width)
			{
				MyWorld::setBrickType(Tag::BRICK);
				positionY = ScreenSize.height - brick->getContentSize().height / 5 * i;
				MyWorld::makeSingleBrick(positionX, positionY,Tag::BRICK);
				positionX = positionX + brick->getContentSize().width / 5;
			}
		}
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
		MyWorld::setBrickType(Tag::BOARD_PROP);
		positionX = ScreenSize.width / 2 - brick->getContentSize().width / 10;
		positionY = ScreenSize.height-brick->getContentSize().height / 5 * 4;
		MyWorld::makeSingleBrick(positionX, positionY,Tag::BOARD_PROP);
		MyWorld::setBrickType(Tag::SCORE_PROP);
		positionX = ScreenSize.width / 2 + brick->getContentSize().width / 10;
		MyWorld::makeSingleBrick(positionX, positionY,Tag::SCORE_PROP);
	}
}

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

void MyWorld::test()
{	
	/*添加小球*/
	ball = Sprite::create("ball.png");
	ball->setScale(0.1f);
	ball->setPosition(ScreenSize.width/2,board->getContentSize().height/10+ ball->getContentSize().width / 20+myFloor->getContentSize().height);
	ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	ballBody->setContactTestBitmask(0xFFFFFFFF);
	ballBody->setGravityEnable(false);
	ballBody->setRotationEnable(false);   //不旋转
	ballBody->setTag(Tag::BALL);          //判别节点
	ballBody->setDynamic(true);
    ballBody->setVelocity(Vec2(originalVelocity, originalVelocity));//设置初始速度

	ball->setPhysicsBody(ballBody);
	this->addChild(ball, 3);               //创建小球并加入到场景中

}

void MyWorld::KeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		if (board->getPosition().x > board->getContentSize().width*boardLength / 2)
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

	}
	else if (bodyA->getTag() == Tag::FLOOR && bodyB->getTag() == Tag::BALL)
	{

	}

	if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BRICK)        //砖块碰撞
	{
		nodeB->removeFromParentAndCleanup(true);
		totalScore += 1 + scoreTag;
	}
	else if (bodyA->getTag() == Tag::BRICK && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		totalScore += 1 + scoreTag;
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
		board->setScale(float(1.5*boardLength), 0.1f);
	}
	else if (bodyA->getTag() == Tag::BOARD_PROP && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		board->setScale(float(1.5*boardLength), 0.1f);
	}
	return true;
}
void MyWorld::update(float dt)     
{
	/*更新板的状态*/
	if (board->getPosition().x < board->getContentSize().width / 20)
	{
		board->setPosition(Vec2(board->getContentSize().width / 20, board->getPosition().y));
	}
	else if (board->getPosition().x > ScreenSize.width - board->getContentSize().width / 20)
	{
		board->setPosition(ScreenSize.width - board->getContentSize().width / 20, board->getPosition().y);
	}

	/*更新小球速度*/
	float Vx = ballBody->getVelocity().x;
	float Vy = ballBody->getVelocity().y;
	float angle = atan(fabs(Vy / Vx));
	float V = float(sqrt(2 * originalVelocity * originalVelocity) + totalScore * 2);
	ballBody->setVelocity(Vec2(Vx>0?V*cos(angle):-V*cos(angle), Vy > 0 ? V * sin(angle) : -V * sin(angle)));
}

/*更新关卡数据*/
void MyWorld::updateChapter()
{
	scoreTag = 0;
	switch (gameLevel)
	{
	case 1:
		amountOfBrick = 20;
		boardLength = 0.25f;
		originalVelocity = 100.0f;
		break;
	case 2:
		amountOfBrick = 25;
		boardLength = 0.15f;
		originalVelocity = 125.0f;
		break;
	case 3:
		amountOfBrick = 30;
		boardLength = 0.1f;
		originalVelocity = 150.0f;
		break;
	default: break;
	}
}




