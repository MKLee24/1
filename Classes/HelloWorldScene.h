#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ChoseLevel.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "vector"
#include "HPLayer.h"
#include "Popup.h"
USING_NS_CC;

#define MAP1_TAG 1
#define MAP2_TAG 2
#define PLAYER_TAG 5
#define FLOOR_TAG 3
#define EDGEONE_TAG 41
#define EDGE_TAG 40
#define BULLET_TAG 6
#define ENEMY_BULLET_TAG 66
#define ENEMY_TAG_ZERO 7
#define ENEMY_TAG_ONE 8
#define HPLAYER_TAG 9

class HelloWorld : public cocos2d::Layer
{
public:
	
	Size winSize;
	Size visibleSize;
	Point origin;
	PhysicsWorld* m_world;
	EventDispatcher* dispatcher;

	player* player1;
	Sprite* map1;
	Sprite* map2;
	ActionInterval* action;
	ActionInterval* mapAction;

	bool isPaused = false;

	bool isPressedD = false;
	bool isPressedA = false;
	bool isPressedW = false;
	bool jumped = false;
	bool isContact = false;

	bool sceneIsPaused = false;

    static Scene* createScene();
	static HelloWorld* create(PhysicsWorld* world, int level);
    virtual bool init(PhysicsWorld* world,int level);

	HPLayer* hpLayer;

	int level;
	float grivity = 600;

	std::vector<bullet*> bullets;
	std::vector<enemy*> enemies;
	std::vector<bullet*> enemyBullets;

	Vector<SpriteFrame*> enemyAttackFrame[ENEMY_ATTACKNUMBER];
	int enemyFrameNumber[LEVELNUMBER][ENEMY_ATTACKNUMBER];
	void setEnemyFrameNumber();

	Vector<SpriteFrame*> playerAttackFrame[ATTACKNUMBER];
	int playerFrameNumber[LEVELNUMBER][ATTACKNUMBER];


	Vector<SpriteFrame*> playerActionFrame[ACTIONNUMBER];
	int playerActionNumber[LEVELNUMBER][ACTIONNUMBER];

	void setPlayerFrameNumber();

	void initDamage();
	std::vector<float> playerDamage;
	std::vector<float> enemyDamage;
	//void setPlayerDamage(int x, float damage);
	//void setEnemyDamage(int x, float damage);

    void menuCloseCallback(cocos2d::Ref* pSender);
	void backToChoseLevel();
	void toPopup();

	void preLoadMusic();
	void playBgMusic();
	void pauseBgMusic();
	void playerEffectMusic(int i);
	void playerParticle(String* plist, String* texture, Point position);

	void preLoadAnimation();

	void enemyComing(float dt);
	
	bullet* throwBullet(Point startPosition,Vec2 force);
	void playerMove(bool isPressedW, bool isPressedA, bool isPressedD,float time);
	void playerAttack(Point touchPos);
	void enemyMove(float time);
	void enemyAttack(float time);
	void particleMove(float time);


	void update(float time);
	void updatePlayerHP(float dt);
	void updateEnemyHP(float dt);
	void updateBullet(float time);
	void updateEnemyBullet(float time);
	void updatePlayerAttackOne(float time);
	void updateEnemyAttack(float time);

	void onTouchEvent();
	void onKeyBoardEvent();
	void customEvent();
	void physicsEvent();

	void particleTest();
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	bool onContactBegin(PhysicsContact& contact);
	void onContactSeperate(PhysicsContact& contact);

	void pauseScene();

	//virtual void onExit();
	//virtual void onEnter();

};

#endif // __HELLOWORLD_SCENE_H__
