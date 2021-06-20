#pragma once
#ifndef __MY_SCENE_H__
#define __MY_SCENE_H__
#include"cocos2d.h"
#include"Box2D\Box2D.h"
#include "ui/CocosGUI.h"     //ui头文件
using namespace cocos2d::ui; //ui命名空间
USING_NS_CC;
class MyWorld:public cocos2d::Layer
{
public:
	MyWorld::MyWorld()
		:originalVelocity(100.0f), boardLength(0.1f), totalScore(0),
		gameLevel(1), amountOfBrick(32), scoreTag(0)
	{ }
	static cocos2d::Scene* createScene();//静态函数，用于获取场景对象
    virtual bool init();       //初始化场景
	virtual void update(float dt);  //更新信息
	void addSprite();          // 添加背景和各种精灵
	void setPhyWorld(PhysicsWorld* world) { m_world = world; }
	void KeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void KeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	void addListener();        //添加监听器
	bool onContactBegin(PhysicsContact& contact);  //碰撞检测,重载系统函数
	void updateChapter();      //更新关卡
	void addScoreLine();       //得分栏
	void makeBricks();         //生成砖块 
	void makeSingleBrick(float& PositionX, float& PositionY, int brickType);    //生产单个砖块
	Sprite* setBrickType(int brickType);             //创建不同类型的砖块精灵
	void write(int insert_number);  //写入得分榜

	CREATE_FUNC(MyWorld);      //使用了一个宏
	~MyWorld() {};


private:
	b2Vec2* world;             //世界
	Sprite* ball;              //小球
	Sprite* board;             //移动的板
	Sprite* brick;             //砖块
	Sprite* myFloor;           //移动的板
	Size ScreenSize;           //屏幕尺寸
	PhysicsWorld* m_world;
	PhysicsBody* ballBody;     //球的刚体
	TextAtlas* scoreText;      //得分文本控件
	float originalVelocity;    //小球初始速度
	float boardLength ;        //板的长度
	int totalScore ;           //总得分
	int gameLevel;             //游戏关卡
	int amountOfBrick;         //砖块数量
	int scoreTag;              //得分道具标志
	enum Tag{BALL,BOARD,BRICK,FLOOR,SCORE_PROP,BOARD_PROP};
};
#endif //__MY_SCENE_H__

