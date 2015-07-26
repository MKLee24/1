#include "HPLayer.h"
USING_NS_CC;

bool HPLayer :: init(){
	if ( !Layer :: init()){
		return false;
	}
	Sprite* HPSprite = Sprite::create("images/HP.png");
	HPProgress = ProgressTimer::create(HPSprite);
	HPProgress->setType(ProgressTimer::Type::BAR);
	HPProgress->setBarChangeRate(Vec2(1,0));
	HPProgress->setMidpoint(Vec2(0, 0));
	HPProgress->setPercentage(100.0f);
	HPProgress->setPosition(HPProgress->getContentSize().width/3*2,0);
	addChild(HPProgress,1,HPTAG);
	return true;
}