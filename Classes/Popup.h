#ifndef _POPUP_H_
#define _POPUP_H_
#include "cocos2d.h"
#include "ChoseLevel.h"
USING_NS_CC;

class Popup : public Layer{
public:
	Size visibleSize;

	Label* contentText;

	static Scene* createScene(RenderTexture* bgTexture, char* bgImage);
	virtual bool init(RenderTexture* bgTexture, char* menuImage);
	static Popup* create(RenderTexture* bgTexture, char* menuImage);
	
	void backToGame();
	void endGame();
	void backToLevel();
	void restart();

	void setContentText(String* str);

};
#endif