#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos2d/cocos/ui/CocosGUI.h"

class HelloWorld : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);

	void update(float delta) override;

	bool circlesToVanish[7][9];
	bool circlesToFall[7][10];
	cocos2d::DrawNode* circles[7][10];
	bool circlesPresent[7][10];
	bool circlesVanishing, circlesFalling, direction;
	bool circlesTurn = false;
	float circlesCoords[7][10][2];
	int circlesColors[7][10];
	cocos2d::Color4F colors[7];
	cocos2d::DrawNode* cursor;
	float cursorCoords[12][8][2];
	int cpx, cpy;
	int pm[12][8][3][2];
	float circleSize;
	int coef = 4;

	void circleCreate(int i, int j);
	void vanishCheck();
	void commonTouch();

	cocos2d::ui::Button* m_rotateRightButton;
	cocos2d::ui::Button* m_rotateLeftButton;
	cocos2d::ui::Button* m_moveRightButton;
	cocos2d::ui::Button* m_moveLeftButton;
	cocos2d::ui::Button* m_moveUpButton;
	cocos2d::ui::Button* m_moveDownButton;

	void rotateRightCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType input);
	void rotateLeftCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType input);
	void moveRightCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType input);
	void moveLeftCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType input);
	void moveUpCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType input);
	void moveDownCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType input);

};

#endif // __HELLOWORLD_SCENE_H__
