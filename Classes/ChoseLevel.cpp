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

	unlock = 2;

	Sprite* bgImage = Sprite::create("images/bg.png");
	bgImage->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(bgImage,1);

	auto homeItem = MenuItemImage::create("images/home.png","images/home.png",CC_CALLBACK_0(ChoseLevel::backHome,this));
	auto homeMenu = Menu::createWithItem(homeItem);
	this->addChild(homeMenu,5);
	homeMenu->setPosition(visibleSize.width-homeItem->getContentSize().width/2, visibleSize.height-homeItem->getContentSize().height/2);

	//auto levelOne = MenuItemLabel::create(Label::createWithTTF("Zero","fonts/arial.ttf",40),
	//													CC_CALLBACK_0(ChoseLevel::selectLevel,this,0));
	//levelOne->setPosition(-2*levelOne->getContentSize().width,0);
	//auto levelTwo = MenuItemLabel::create(Label::createWithTTF("One", "fonts/arial.ttf", 40),
	//													CC_CALLBACK_0(ChoseLevel::selectLevel, this, 1));
	//levelTwo->setPosition(0,0);
	//auto levelThree = MenuItemLabel::create(Label::createWithTTF("Two", "fonts/arial.ttf", 40),
	//													CC_CALLBACK_0(ChoseLevel::selectLevel, this, 2));
	//levelThree->setPosition(2 * levelThree->getContentSize().width, 0);
	//auto choseMenu = Menu::create(levelOne,levelTwo,levelThree,NULL);

	//choseMenu->setPosition(visibleSize.width/2,visibleSize.height/2);
	//this->addChild(choseMenu,2);

	auto menuBg = Sprite::create("images/scroll.png");
	menuBg->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(menuBg,2);

	Vector<MenuItem*> levels;
	MenuItemSprite* levelItem;
	/*
	for (int i = 0; i < unlock; i++){
		auto normalSprite = Sprite::create("images/levelImage.png");
		auto selectedSprite = Sprite::create("images/levelImage.png");
		selectedSprite->setOpacity(200);
		levelItem = MenuItemSprite::create(normalSprite, selectedSprite, CC_CALLBACK_0(ChoseLevel::selectLevel,this,i));
		levelItem->setPosition(i*levelItem->getContentSize().width*3/2,0);
		
		auto levelStr = String::createWithFormat("%d",i+1);
		auto levelLabel = Label::createWithTTF(levelStr->getCString(),"fonts/arial.ttf",40);
		levelLabel->setPosition(levelItem->getContentSize().width/2, levelItem->getContentSize().height/2);
		levelItem->addChild(levelLabel);
		levels.pushBack(levelItem);
	}*/
	
	int count = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++){
			auto normalSprite = Sprite::create("images/levelImage.png");
			auto selectedSprite = Sprite::create("images/levelImage.png");
			selectedSprite->setOpacity(200);
			levelItem = MenuItemSprite::create(normalSprite, selectedSprite);
			levelItem->setPosition(i*levelItem->getContentSize().width*3/2, -j*levelItem->getContentSize().height*3/2);

			if (count < unlock){
				levelItem->setCallback(CC_CALLBACK_0(ChoseLevel::selectLevel, this, i));
				auto levelStr = String::createWithFormat("%d", count + 1);
				auto levelLabel = Label::createWithTTF(levelStr->getCString(), "fonts/arial.ttf", 40);
				levelLabel->setPosition(levelItem->getContentSize().width / 2, levelItem->getContentSize().height / 2);
				levelItem->addChild(levelLabel);
				count++;
			}
			else{
				auto lock = Sprite::create("images/locked.png");
				lock->setPosition(levelItem->getContentSize().width / 2, levelItem->getContentSize().height / 2);
				levelItem->addChild(lock);
			}

			levels.pushBack(levelItem);
		}
	}

	auto levelMenu = Menu::createWithArray(levels);
	levelMenu->setPosition(visibleSize.width / 2 - levelItem->getContentSize().width * 3 / 2, visibleSize.height / 2 + levelItem->getContentSize().width/10*7);
	this->addChild(levelMenu,3);

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

void ChoseLevel::backHome(){
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}