#ifndef _CHOSELEVEL_
#define _CHOSELEVEL_
#include <cocos2d.h>
#include "HelloWorldScene.h"
USING_NS_CC;

class ChoseLevel : public Layer
{
public:
	Size visibleSize;
	Point origin;
	static Scene* createScene();
	static ChoseLevel* create(PhysicsWorld* world);
	virtual bool init(PhysicsWorld* world);
	PhysicsWorld* m_world;
	void selectLevel(int selectedLevel);
};
#endif