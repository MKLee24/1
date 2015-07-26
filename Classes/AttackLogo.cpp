#include "AttackLogo.h"
USING_NS_CC;

Scene* SecondScene::createScene(){
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = SecondScene::create(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

SecondScene* SecondScene::create(PhysicsWorld* world){
	SecondScene* pRet = new SecondScene();
	if (pRet && pRet->init(world)){
		return pRet;
	}
	pRet = NULL;
	return pRet;
}

bool SecondScene::init(PhysicsWorld* world){
	if (!Layer::init()){
		return false;
	}
	m_world = world;
	winSize = Director::getInstance()->getWinSize();
	Sprite* temp = Sprite::create("box.png");
	temp->setPosition(winSize.width/2,winSize.height/2);
	this->addChild(temp);

	return true;
}