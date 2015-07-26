#include "choseLevel.h"
USING_NS_CC;

Scene* ChoseLevel::createScene(){
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto Layer = ChoseLevel::create(scene->getPhysicsWorld());
	scene->addChild(Layer);
	return scene;
}

ChoseLevel* ChoseLevel::create(PhysicsWorld* world){
	ChoseLevel* pRet = new ChoseLevel();
	if (pRet && pRet->init(world)){
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

bool ChoseLevel::init(PhysicsWorld* world){
	if (!Layer::init()){
		return false;
	}
	m_world = world;
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	Sprite* bgImage = Sprite::create("images/bg.png");
	bgImage->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(bgImage,1);

	auto levelOne = MenuItemLabel::create(Label::createWithTTF("Zero","fonts/arial.ttf",40),
														CC_CALLBACK_0(ChoseLevel::selectLevel,this,0));
	levelOne->setPosition(-2*levelOne->getContentSize().width,0);
	auto levelTwo = MenuItemLabel::create(Label::createWithTTF("One", "fonts/arial.ttf", 40),
														CC_CALLBACK_0(ChoseLevel::selectLevel, this, 1));
	levelTwo->setPosition(0,0);
	auto levelThree = MenuItemLabel::create(Label::createWithTTF("Two", "fonts/arial.ttf", 40),
														CC_CALLBACK_0(ChoseLevel::selectLevel, this, 2));
	levelThree->setPosition(2 * levelThree->getContentSize().width, 0);
	auto choseMenu = Menu::create(levelOne,levelTwo,levelThree,NULL);

	choseMenu->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(choseMenu,2);

	return true;
}

void ChoseLevel::selectLevel(int SelectedLevel){
	auto levelData = String::createWithFormat("%d",SelectedLevel);
	log(levelData->getCString());
	//NotificationCenter::getInstance()->postNotification("selectedLevel", levelData);
	auto scene = HelloWorld::createScene();
	auto layer = HelloWorld::create(scene->getPhysicsWorld(),SelectedLevel);
	scene->addChild(layer);
	Director::getInstance()->replaceScene(CCTransitionCrossFade::create(1.0f,scene));
}