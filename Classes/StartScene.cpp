#include "StartScene.h"
USING_NS_CC;

Scene* StartScene::createScene(){
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init(){
	if (!Layer::init()){
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	auto bgSprite = Sprite::create("images/bg.png");
	bgSprite->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(bgSprite,1);

	
	auto startItem = MenuItemImage::create("images/startButton.png","images/startButton.png",CC_CALLBACK_0(StartScene::toChoseLevel,this));
	auto startLabel = Label::createWithTTF("Start Game", "fonts/arial.ttf", 25);
	startItem->addChild(startLabel);
	startLabel->setPosition(startItem->getContentSize().width/3,startItem->getContentSize().height/2+startLabel->getContentSize().height/3);
	auto helpItem = MenuItemImage::create("images/startButton.png", "images/startButton.png", CC_CALLBACK_0(StartScene::toHelp,this));
	auto helpLabel= Label::createWithTTF("Introduce", "fonts/arial.ttf", 25);
	helpItem->addChild(helpLabel);
	helpLabel->setPosition(startItem->getContentSize().width / 3, startItem->getContentSize().height / 2 + startLabel->getContentSize().height / 3);
	auto settingItem = MenuItemImage::create("images/startButton.png", "images/startButton.png", CC_CALLBACK_0(StartScene::toSetting,this));
	auto settingLabel = Label::createWithTTF("Setting", "fonts/arial.ttf", 25);
	settingItem->addChild(settingLabel);
	settingLabel->setPosition(startItem->getContentSize().width / 3, startItem->getContentSize().height / 2 + startLabel->getContentSize().height / 3);

	startItem->setPosition(0,helpItem->getContentSize().height);
	settingItem->setPosition(0,-helpItem->getContentSize().height);
	helpItem->setPosition(0,0);
	auto startMenu = Menu::create(startItem,helpItem,settingItem,NULL);
	startMenu->setPosition(visibleSize.width/3, visibleSize.height/3);
	addChild(startMenu,2);
	

	return true;
}

void StartScene::toChoseLevel(){
	auto scene = ChoseLevel::createScene();
	Director::getInstance()->replaceScene(scene);
}

void StartScene::toHelp(){
	auto director = Director::getInstance();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto popup = Popup::createScene(renderTexture,"images/popupBg.png");
	//this->addChild(popup, 10);
	//Action* popupAction = Sequence::create(ScaleTo::create(0.0f, 0.0f),
	//	ScaleTo::create(0.06f, 1.05f),
	//	ScaleTo::create(0.08f, 0.95f),
	//	ScaleTo::create(0.08f, 1.0f), NULL);
	//popup->runAction(popupAction);
	director->pushScene(popup);
	auto str = String::create("±³¾°½éÉÜ£¬ÓÎÏ·²Ù×÷½éÉÜ");
	//popup->setContentText(str);
}

void StartScene::toSetting(){

}
