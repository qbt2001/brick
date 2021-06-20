#include "OnlineScene.h"
#include<cmath>
#include "../win32-build/GameOver.h"
#include "../win32-build/Next.h"
#include "../win32-build/Win.h"
#include "../win32-build/Lost.h"
#define pi 3.14159

USING_NS_CC;

//联机版游戏的代码实现，此程序为客户端
float positionx1 = 0, positionx2 = 0, positiony1 = 0, positiony2 = 0;

int getChapter()
{
	static int GameLevel = 0;
	GameLevel++;
	return GameLevel;
}

constexpr int numberOfChar(char ch) {
	return static_cast<int>(ch) - static_cast<int>('0');
}

Scene* OnlineScene::createScene()
{
	auto scene = Scene::createWithPhysics();   //创建一个场景对象
	auto layer = OnlineScene::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f));
	scene->addChild(layer);                   //将layer加入到场景scene中去
	return scene;
}
bool OnlineScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	ScreenSize = Director::getInstance()->getVisibleSize();
	ScreenSize.width /= 2;
	/*设置边界*/
	auto edgeBody = PhysicsBody::createEdgeBox(ScreenSize, PhysicsMaterial(100.0f,1.0f,1.0f),0.1f);     //三参数代表矩形边界的大小、材质(密度、恢复系数、摩擦力)、边框宽度
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(ScreenSize.width / 2, ScreenSize.height / 2));
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);

	WSADATA wsadata;
	sockaddr_in serveraddr;

	WSAStartup(MAKEWORD(2, 2), &wsadata);
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(12345);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(12346);
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(client, (sockaddr*)&clientaddr, sizeof(clientaddr));

	int ret = connect(client, (sockaddr*)&serveraddr, sizeof(serveraddr));
	connect(client, (sockaddr*)&serveraddr, sizeof(serveraddr));

	gameLevel = getChapter();

	OnlineScene::updateChapter();
	OnlineScene::addSprite();
	OnlineScene::addListener();
	OnlineScene::test();
	OnlineScene::makeBricks();

	copy_board = Sprite::create("board.png");
	copy_board->setScale(boardLength, 0.1f);
	copy_board->setPosition(ScreenSize.width / 2 * 3, 0);
	this->addChild(copy_board, 7);
	copy_ball = Sprite::create("ball.png");
	copy_ball->setScale(0.1f);
	copy_ball->setPosition(ScreenSize.width / 2 * 3, board->getContentSize().height / 10 + ball->getContentSize().width / 20 + myFloor->getContentSize().height);
	this->addChild(copy_ball, 7);

	Node::schedule(CC_SCHEDULE_SELECTOR(OnlineScene::update), 0.01f, kRepeatForever, 0.01f);

	return true;
}

void OnlineScene::addSprite()
{
	/*添加背景*/
	auto* backGround = Sprite::create("bg.jpg");
	backGround->setPosition(ScreenSize.width / 2, ScreenSize.height / 2);
	backGround->setScale(ScreenSize.width / backGround->getContentSize().width, ScreenSize.height / backGround->getContentSize().height);
	addChild(backGround,0);                   //参数0表示drawing优先级

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
	board->setPosition(ScreenSize.width / 2, myFloor->getContentSize().height * 0.1f);
	auto boardBody = PhysicsBody::createBox(board->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 10.0f));
	boardBody->setContactTestBitmask(0xFFFFFFFF);
	boardBody->setRotationEnable(false);   //不旋转
	boardBody->setTag(Tag::BOARD);         //判断节点
	boardBody->setDynamic(false);
	board->setPhysicsBody(boardBody);
	this->addChild(board);              //创建底板并加入到场景中

}

Sprite* OnlineScene::setBrickType(int brickType)
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
void OnlineScene::makeSingleBrick(float& PositionX,float& PositionY,int brickType)
{
	brick->setPosition(PositionX, PositionY);

	auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(1000.0f, 1.0f, 0.0f));
	brickBody->setContactTestBitmask(0xFFFFFFFF);
	brickBody->setRotationEnable(false);   //不旋转
	brickBody->setTag(brickType);         //判断节点
	brickBody->setDynamic(false);

	brick->setPhysicsBody(brickBody);
	this->addChild(brick, 2);

}

void OnlineScene::make(float& PositionX, float& PositionY)
{
	Size size = Director::getInstance()->getVisibleSize();
	Sprite* sprite = Sprite::create("brick.png");
	sprite->setScale(0.2f, 0.2f);
	sprite->setPosition(PositionX+size.width/2, PositionY);
	this->addChild(sprite,5);
}
void OnlineScene::makeBricks()
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
				OnlineScene::setBrickType(Tag::BRICK);
				positionY = ScreenSize.height - brick->getContentSize().height / 5 * i;
				OnlineScene::makeSingleBrick(positionX, positionY,Tag::BRICK);
				OnlineScene::make(positionX, positionY);
				positionX = positionX + brick->getContentSize().width / 5;
			}
		}
	}
	
}

void OnlineScene::addListener()
{
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(OnlineScene::KeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(OnlineScene::KeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//add contact event listerner
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(OnlineScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void OnlineScene::test()
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

void OnlineScene::KeyPressed(EventKeyboard::KeyCode keycode, Event* event)
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

void OnlineScene::KeyReleased(EventKeyboard::KeyCode keycode, Event* event)
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
bool OnlineScene::onContactBegin(PhysicsContact& contact)
{

	auto bodyA = contact.getShapeA()->getBody();
	auto nodeA = bodyA->getNode();
	auto bodyB = contact.getShapeB()->getBody();
	auto nodeB = bodyB->getNode();

	if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::FLOOR)        //判断游戏结束
	{
		this->unscheduleUpdate();
		std::string senddata = "win";
		send(client, senddata.c_str(), sizeof(senddata), 0);
		Director::getInstance()->replaceScene(Lost::createScene());
		return 1;
	}
	else if (bodyA->getTag() == Tag::FLOOR && bodyB->getTag() == Tag::BALL)
	{
		this->unscheduleUpdate();
		std::string senddata = "win";
		send(client, senddata.c_str(), sizeof(senddata), 0);
		Director::getInstance()->replaceScene(Lost::createScene());
		return 1;
	}

	if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BRICK)        //砖块碰撞
	{
		nodeB->removeFromParentAndCleanup(true);
		positionx2 = positionx1, positionx1 = bodyB->getPosition().x;
		positiony2 = positiony1, positiony1 = bodyB->getPosition().y;
		amountOfBrick--;
	}
	else if (bodyA->getTag() == Tag::BRICK && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		positionx2 = positionx1, positionx1 = bodyA->getPosition().x;
		positiony2 = positiony1, positiony1 = bodyA->getPosition().y;
		amountOfBrick--;
	}
	else if (bodyA->getTag() == Tag::SCORE_PROP && bodyB->getTag() == Tag::BALL)//得分道具
	{
		nodeA->removeFromParentAndCleanup(true);
		positionx2 = positionx1, positionx1 = bodyA->getPosition().x;
		positiony2 = positiony1, positiony1 = bodyA->getPosition().y;
		scoreTag = 1;
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::SCORE_PROP)
	{
		nodeB->removeFromParentAndCleanup(true);
		positionx2 = positionx1, positionx1 = bodyB->getPosition().x;
		positiony2 = positiony1, positiony1 = bodyB->getPosition().y;
		scoreTag = 1;
	}
	else if (bodyA->getTag() == Tag::BALL && bodyB->getTag() == Tag::BOARD_PROP)//底板增长道具
	{
		nodeB->removeFromParentAndCleanup(true);
		positionx2 = positionx1, positionx1 = bodyB->getPosition().x;
		positiony2 = positiony1, positiony1 = bodyB->getPosition().y;
		boardLength *= 1.4f;
		board->setScale(boardLength, 0.1f);
	}
	else if (bodyA->getTag() == Tag::BOARD_PROP && bodyB->getTag() == Tag::BALL)
	{
		nodeA->removeFromParentAndCleanup(true);
		positionx2 = positionx1, positionx1 = bodyA->getPosition().x;
		positiony2 = positiony1, positiony1 = bodyA->getPosition().y;
		boardLength *= 1.4f;
		board->setScale(boardLength, 0.1f);
	}

	if (0 == amountOfBrick && 1 == gameLevel)
	{
		this->unscheduleUpdate();
		std::string senddata = "end";
		send(client, senddata.c_str(), sizeof(senddata), 0);
		Director::getInstance()->replaceScene(Win::createScene());
		return 1;
	}
	

	return true;
}
void OnlineScene::update(float dt)     
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
	ballBody->setVelocity(Vec2(Vx>0?V*cos(angle):-V*cos(angle), Vy > 0 ? V * sin(angle) : -V * sin(angle)));

	int boardx = board->getPosition().x;
	int boardy = board->getPosition().y;
	int ballx = ball->getPosition().x;
	int bally = ball->getPosition().y;
	std::string senddata;
	senddata = std::to_string(boardx) + ' ';
	senddata = senddata + std::to_string(boardy) + ' ';
	senddata = senddata + std::to_string(ballx) + ' ';
	senddata = senddata + std::to_string(bally) + ' ';
	send(client, senddata.c_str(), sizeof(senddata), 0);

	char recvdata[1024] = { 0 };
	recv(client, recvdata, 1023, 0);
	if (recvdata[0] == 'e')
	{
		this->unscheduleUpdate();
		closesocket(client);
		WSACleanup();
		Director::getInstance()->replaceScene(Lost::createScene());
		return;
	}
	if (recvdata[0] == 'w')
	{
		this->unscheduleUpdate();
		closesocket(client);
		WSACleanup();
		Director::getInstance()->replaceScene(Win::createScene());
		return;
	}
	std::vector<int> position_ = { 0,0,0,0,0,0,0,0,0 };
	int number = 0, tag_ = 0, position_char = 0;
	while (tag_ < 4)
	{
		if (recvdata[position_char] == ' ' || recvdata[position_char] == '\0')
		{
			position_[tag_] = number;
			number = 0;
			tag_++;
		}
		else
			number = number * 10 + numberOfChar(recvdata[position_char]);
		position_char++;
	}
	copy_board->setPosition(position_[0] + ScreenSize.width, position_[1]);
	copy_ball->setPosition(position_[2] + ScreenSize.width, position_[3]);



	senddata = "";
	int px1 = positionx1, py1 = positiony1, px2 = positionx2, py2 = positiony2;
	positionx1 = positionx2 = positiony1 = positiony2 = 0;
	senddata = std::to_string(px1) + ' ';
	senddata = senddata + std::to_string(py1) + ' ';
	senddata = senddata + std::to_string(px2) + ' ';
	senddata = senddata + std::to_string(py2) + ' ';
	send(client, senddata.c_str(), sizeof(senddata), 0);

	recv(client, recvdata, 1023, 0);
	if (recvdata[0] == 'e')
	{
		this->unscheduleUpdate();
		closesocket(client);
		WSACleanup();
		Director::getInstance()->replaceScene(Lost::createScene());
		return;
	}
	if (recvdata[0] == 'w')
	{
		this->unscheduleUpdate();
		closesocket(client);
		WSACleanup();
		Director::getInstance()->replaceScene(Win::createScene());
		return;
	}
	position_[8] = 0, number = 0;
	tag_ = 4, position_char = 0;
	while (tag_ < 8)
	{
		if (recvdata[position_char] == ' ' || recvdata[position_char] == '\0')
		{
			position_[tag_] = number;
			number = 0;
			tag_++;
		}
		else
			number = number * 10 + numberOfChar(recvdata[position_char]);
		position_char++;
	}
	int px_1 = position_[4], py_1 = position_[5], px_2 = position_[6], py_2 = position_[7];
	if (abs(px_1) > 0.01 && abs(py_1) > 0.01)
	{
		Size size = Director::getInstance()->getVisibleSize();
		Sprite* sprite = Sprite::create("brickwhite.png");
		sprite->setScale(0.201f, 0.201f);
		sprite->setPosition(px_1 + size.width / 2, py_1);
		this->addChild(sprite, 6);
	}
	if (abs(px_2) > 0.01 && abs(py_2) > 0.01)
	{
		Size size = Director::getInstance()->getVisibleSize();
		Sprite* sprite = Sprite::create("brickwhite.png");
		sprite->setScale(0.201f, 0.201f);
		sprite->setPosition(px_2 + size.width / 2, py_2);
		this->addChild(sprite, 6);
	}

	
}

/*更新关卡数据*/
void OnlineScene::updateChapter()
{
	scoreTag = 0;
	switch (gameLevel)
	{
	case 1:
		amountOfBrick = 24;
		boardLength = 0.08f;
		originalVelocity = 100.0f;
		break;
	default: break;
	}
}




