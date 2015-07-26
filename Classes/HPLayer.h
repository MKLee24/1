#include "cocos2d.h"
USING_NS_CC;

#define HPTAG 10

class HPLayer : public Layer{
public:
	virtual bool init();
	ProgressTimer* HPProgress;
	CREATE_FUNC(HPLayer);
};