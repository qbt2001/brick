#pragma once
#ifndef __MY_SCENE_H__
#define __MY_SCENE_H__
#include"cocos2d.h"
#include"Box2D\Box2D.h"
USING_NS_CC;
class MyWorld:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();//��̬���������ڻ�ȡ��������
    virtual bool init();       //��ʼ������
	virtual void update(float dt);  //������Ϣ
	void addSprite();          // ��ӱ����͸��־���
	void setPhyWorld(PhysicsWorld* world) { m_world = world; }
	void test();               //��������
	void KeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void KeyReleased(EventKeyboard::KeyCode keycode, Event* event);
	void addListener();        //��Ӽ�����
	bool onContactBegin(PhysicsContact& contact);  //��ײ���
	void makeBricks() ;        //����ש�� 
	void updateChapter();      //���¹ؿ�
	void makeSingleBrick(float& PositionX,float& PositionY,int brickType);    //��������ש��
	Sprite* setBrickType(int brickType);             //������ͬ���͵�ש�龫��
	CREATE_FUNC(MyWorld);      //ʹ����һ����

private:
	b2Vec2* world;             //����
	Sprite* ball;              //С��
	Sprite* board;             //�ƶ��İ�
	Sprite* brick;             //ש��
	Sprite* myFloor;            //�ƶ��İ�
	Size ScreenSize;           //��Ļ�ߴ�
	PhysicsWorld* m_world;
	PhysicsBody* ballBody;     //��ĸ���
	int totalScore = 0;        //�ܵ÷�
	float originalVelocity = 50.0f;//С���ʼ�ٶ�
	float boardLength = 0.4f;  //��ĳ���
	int gameLevel = 2;         //��Ϸ�ؿ�
	int amountOfBrick=20;      //ש������
	int scoreTag = 0;          //�÷ֵ��߱�־
	enum Tag{BALL,BOARD,BRICK,FLOOR,SCORE_PROP,BOARD_PROP};

protected:


};
#endif //__MY_SCENE_H__

