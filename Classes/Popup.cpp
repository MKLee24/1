#include "Popup.h"
USING_NS_CC;
#pragma execution_character_set("utf-8")
/*
Scene* Popup::createScene(RenderTexture* bgTexture,char* menuImage){
	Scene* scene = Scene::create();
	auto layer = Popup::create(bgTexture,menuImage);
	scene->addChild(layer,1);
	return scene;
}
*/

Popup* Popup::create( char* menuImage){
	auto layer = new Popup();
	if (layer && layer->init(menuImage)){
		return layer;
	}
	layer = NULL;
	return NULL;
}

bool Popup::init(char* menuImage){
	if (!Layer::init()){
		return false;
	}

	//auto bgSprite = Sprite::createWithTexture(bgTexture->getSprite()->getTexture());
	//bgSprite->setFlippedY(true);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//this->addChild(bgSprite, 1);
	auto menuSprite = Sprite::create(menuImage);
	menuSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuSprite, 2);

	auto contentText = Label::createWithSystemFont("µ¯´°²âÊÔ","Î¢ÈíÑÅºÚ",20);
	contentText->setColor(Color3B::BLACK);
	//title->setContentSize(Size(menuSprite->getContentSize().width-padding.x, menuSprite->getContentSize().height-padding.y));
	contentText->setAnchorPoint(Point(0.5,0.5));
	contentText->setPosition(menuSprite->getContentSize().width/2, menuSprite->getContentSize().height/3*2);
	menuSprite->addChild(contentText,1);

	auto Yes = Label::createWithSystemFont("È·¶¨","Î¢ÈíÑÅºÚ",20);
	Yes->setColor(Color3B::BLACK);
	auto YesItem = MenuItemLabel::create(Yes,CC_CALLBACK_0(Popup::backToGame,this));
	YesItem->setPosition(-YesItem->getContentSize().width,0);

	auto No = Label::createWithSystemFont("È¡Ïû","Î¢ÈíÑÅºÚ",20);
	No->setColor(Color3B::BLACK);
	auto NoItem = MenuItemLabel::create(No,CC_CALLBACK_0(Popup::endGame,this));
	NoItem->setPosition(NoItem->getContentSize().width,0);

	auto menu = Menu::create(YesItem,NoItem,NULL);
	menu->setPosition(menuSprite->getContentSize().width/2, YesItem->getContentSize().height*2);
	menuSprite->addChild(menu,2);

	return true;
}

void Popup::backToGame(){
	Action* popAction = Sequence::create(ScaleTo::create(0.08f, 1.0f), 
																ScaleTo::create(0.08f, 0.95f), 
																ScaleTo::create(0.06f, 1.05f),
																ScaleTo::create(0.0f, 0.0f), NULL);
	this->runAction(popAction);
	this->removeFromParentAndCleanup(true);
}

void Popup::endGame(){
	Director::getInstance()->end();
}