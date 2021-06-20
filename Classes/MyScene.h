#pragma once
#ifndef __MY_SCENE_H__
#define __MY_SCENE_H__
#include"cocos2d.h"
#include"Box2D\Box2D.h"
#include "ui/CocosGUI.h"     //uiͷ�ļ�
using namespace cocos2d::ui; //ui�����ռ�
USING_NS_CC;
class MyWorld:public cocos2d::Layer
{
public:
	MyWorld::MyWorld()
		:originalVelocity(100.0f), boardLength(0.1f), totalScore(0),
		gameLevel(1), amountOfBrick(32), scoreTag(0)
	{ }
	static cocos2d::Scene* createScene();//��̬���������ڻ�ȡ��������
    virtual bool init();       //��ʼ������
	virtual void update(float dt);  //������Ϣ
	void addSprite();          // ��ӱ����͸��־���
	void setPhyWorld(PhysicsWorld* world) { m_world = world; }
	void KeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void KeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	void addListener();        //��Ӽ�����
	bool onContactBegin(PhysicsContact& contact);  //��ײ���,����ϵͳ����
	void updateChapter();      //���¹ؿ�
	void addScoreLine();       //�÷���
	void makeBricks();         //����ש�� 
	void makeSingleBrick(float& PositionX, float& PositionY, int brickType);    //��������ש��
	Sprite* setBrickType(int brickType);             //������ͬ���͵�ש�龫��
	void write(int insert_number);  //д��÷ְ�

	CREATE_FUNC(MyWorld);      //ʹ����һ����
	~MyWorld() {};


private:
	b2Vec2* world;             //����
	Sprite* ball;              //С��
	Sprite* board;             //�ƶ��İ�
	Sprite* brick;             //ש��
	Sprite* myFloor;           //�ƶ��İ�
	Size ScreenSize;           //��Ļ�ߴ�
	PhysicsWorld* m_world;
	PhysicsBody* ballBody;     //��ĸ���
	TextAtlas* scoreText;      //�÷��ı��ؼ�
	float originalVelocity;    //С���ʼ�ٶ�
	float boardLength ;        //��ĳ���
	int totalScore ;           //�ܵ÷�
	int gameLevel;             //��Ϸ�ؿ�
	int amountOfBrick;         //ש������
	int scoreTag;              //�÷ֵ��߱�־
	enum Tag{BALL,BOARD,BRICK,FLOOR,SCORE_PROP,BOARD_PROP};
};
#endif //__MY_SCENE_H__

