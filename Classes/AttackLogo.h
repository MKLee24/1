#ifndef _SECONDSCENE_H_
#define _SECONDSCENE_H_
#include "HelloWorldScene.h"
USING_NS_CC;

class SecondScene : public Layer
{
public:
	Size winSize;
	static Scene* createScene();
	static SecondScene* create(PhysicsWorld* world);
	virtual bool init(PhysicsWorld* world);
	PhysicsWorld* m_world;
};

#endif