#ifndef _STARTSCENE_H_
#define _STARTSCENE_H_
#include "cocos2d.h"
#include "ChoseLevel.h"
#include "Popup.h"
USING_NS_CC;

#pragma execution_character_set("utf-8")

class StartScene : public Layer{
public:
	Size visibleSize;
	virtual bool init();
	static Scene* createScene();

	void toChoseLevel();
	void toHelp();
	void toSetting();

	CREATE_FUNC(StartScene);
};
#endif