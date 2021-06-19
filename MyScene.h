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
	static cocos2d::Scene* createScene();//静态函数，用于获取场景对象
    virtual bool init();       //初始化场景
	virtual void update(float dt);  //更新信息
	void addSprite();          // 添加背景和各种精灵
	void setPhyWorld(PhysicsWorld* world) { m_world = world; }
	void test();               //测试所用
	void KeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void KeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	void addListener();        //添加监听器
	bool onContactBegin(PhysicsContact& contact);  //碰撞检测
	void makeBricks() ;        //生成砖块 
	void updateChapter();      //更新关卡
	void addScoreLine();
	void makeSingleBrick(float& PositionX,float& PositionY,int brickType);    //生产单个砖块
	Sprite* setBrickType(int brickType);             //创建不同类型的砖块精灵
	CREATE_FUNC(MyWorld);      //使用了一个宏
	~MyWorld() {};


private:
	b2Vec2* world;             //世界
	Sprite* ball;              //小球
	Sprite* board;             //移动的板
	Sprite* brick;             //砖块
	Sprite* myFloor;            //移动的板
	Size ScreenSize;           //屏幕尺寸
	PhysicsWorld* m_world;
	PhysicsBody* ballBody;     //球的刚体
	TextAtlas* scoreText;
	float originalVelocity = 50.0f;//小球初始速度
	float boardLength = 0.4f;  //板的长度
	int totalScore = 0;        //总得分
	int gameLevel = 1;         //游戏关卡
	int amountOfBrick=20;      //砖块数量
	int scoreTag = 0;          //得分道具标志
	enum Tag{BALL,BOARD,BRICK,FLOOR,SCORE_PROP,BOARD_PROP,HARD_BRICK};
};
#endif //__MY_SCENE_H__

