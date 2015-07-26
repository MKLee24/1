#include "cocos2d.h"
USING_NS_CC;

class Popup : public Layer{
public:
	Size visibleSize;
	//static Scene* createScene(RenderTexture* bgTexture, char* bgImage);
	virtual bool init( char* menuImage);
	static Popup* create( char* menuImage);
	
	void backToGame();
	void endGame();
};