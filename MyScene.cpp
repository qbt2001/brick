#include "MyScene.h"
#include<cmath>
#define pi 3.14159

USING_NS_CC;
Scene* MyWorld::createScene()
{
	auto scene = Scene::createWithPhysics();   //����һ����������
	auto layer = MyWorld::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f));
	scene->addChild(layer);                   //��layer���뵽����scene��ȥ
	return scene;
}
bool MyWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}
	ScreenSize = Director::getInstance()->getVisibleSize();

	/*���ñ߽�*/
	auto edgeBody = PhysicsBody::createEdgeBox(ScreenSize, PhysicsMaterial(100.0f,1.0f,1.0f),0.1f);     //������������α߽�Ĵ�С������(�ܶȡ��ָ�ϵ����Ħ����)���߿���
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
	/*��ӱ���*/
	auto* backGround = Sprite::create("bg.jpg");
	backGround->setPosition(ScreenSize.width / 2, ScreenSize.height / 2);
	backGround->setScale(ScreenSize.width / backGround->getContentSize().width, ScreenSize.height / backGround->getContentSize().height);
	addChild(backGround,0);                   //����0��ʾdrawing���ȼ�

	/*��ӵװ�*/
	board = Sprite::create("board.png");
	board->setScale(boardLength, 0.1f);
	board->setPosition(ScreenSize.width / 2, 0);
	auto boardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
	boardBody->setContactTestBitmask(0xFFFFFFFF);
	boardBody->setRotationEnable(false);   //����ת
	boardBody->setTag(Tag::BOARD);         //�жϽڵ�
	boardBody->setDynamic(false);
	board->setPhysicsBody(boardBody);
	this->addChild(board, 1);              //�����װ岢���뵽������

	/*��ӵذ�*/
	myFloor= Sprite::create("floor.png");
	myFloor->setPosition(ScreenSize.width / 2, 0);
	myFloor->setScale(ScreenSize.width / myFloor->getContentSize().width, myFloor->getContentSize().height);
	auto floorBody = PhysicsBody::createBox(myFloor->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	floorBody->setRotationEnable(false);   //����ת
	floorBody->setTag(Tag::FLOOR);         //�жϽڵ�
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

/*���ɵ���ש��
������λ������X,λ������Y
ש������*/
void MyWorld::makeSingleBrick(float& PositionX,float& PositionY,int brickType)
{
	brick->setPosition(PositionX,PositionY);

	auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	brickBody->setContactTestBitmask(0xFFFFFFFF);
	brickBody->setRotationEnable(false);   //����ת
	brickBody->setTag(brickType);         //�жϽڵ�
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
	/*���С��*/
	ball = Sprite::create("ball.png");
	ball->setScale(0.1f);
	ball->setPosition(ScreenSize.width/2,board->getContentSize().height/10+ ball->getContentSize().width / 20+myFloor->getContentSize().height);
	ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	ballBody->setContactTestBitmask(0xFFFFFFFF);
	ballBody->setGravityEnable(false);
	ballBody->setRotationEnable(false);   //����ת
	ballBody->setTag(Tag::BALL);          //�б�ڵ�
	ballBody->setDynamic(true);
    ballBody->setVelocity(Vec2(originalVelocity, originalVelocity));//���ó�ʼ�ٶ�

	ball->setPhysicsBody(ballBody);
	this->addChild(ball, 3);               //����С�򲢼��뵽������

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

/*��ײ��⺯��*/
bool MyWorld::onContactBegin(PhysicsContact& contact)
{
	
	auto bodyA = contact.getShapeA()->getBody();
	auto nodeA = bodyA->getNode();
	auto bodyB = contact.getShapeB()->getBody();
	auto nodeB = bodyB->getNode();

	if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::FLOOR)        //�ж���Ϸ����
	{

	}
	else if (bodyA->getTag() == Tag::FLOOR && bodyB->getTag() == Tag::BALL)
	{

	}

	if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BRICK)        //ש����ײ
	{
		nodeB->removeFromParentAndCleanup(true);
		totalScore += 1 + scoreTag;
	}
	else if (bodyA->getTag() == Tag::BRICK && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		totalScore += 1 + scoreTag;
	}
	else if (bodyA->getTag() == Tag::SCORE_PROP && bodyB->getTag() == Tag::BALL)//�÷ֵ���
	{
		nodeA->removeFromParentAndCleanup(true);
		scoreTag = 1;
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::SCORE_PROP)
	{
		nodeB->removeFromParentAndCleanup(true);
		scoreTag = 1;
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BOARD_PROP)//�װ���������
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
	/*���°��״̬*/
	if (board->getPosition().x < board->getContentSize().width / 20)
	{
		board->setPosition(Vec2(board->getContentSize().width / 20, board->getPosition().y));
	}
	else if (board->getPosition().x > ScreenSize.width - board->getContentSize().width / 20)
	{
		board->setPosition(ScreenSize.width - board->getContentSize().width / 20, board->getPosition().y);
	}

	/*����С���ٶ�*/
	float Vx = ballBody->getVelocity().x;
	float Vy = ballBody->getVelocity().y;
	float angle = atan(fabs(Vy / Vx));
	float V = float(sqrt(2 * originalVelocity * originalVelocity) + totalScore * 2);
	ballBody->setVelocity(Vec2(Vx>0?V*cos(angle):-V*cos(angle), Vy > 0 ? V * sin(angle) : -V * sin(angle)));
}

/*���¹ؿ�����*/
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




