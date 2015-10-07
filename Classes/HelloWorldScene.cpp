#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	circleSize = visibleSize.height / (float)((float)8 * (float)sqrt(3) / 2 + 1);

	colors[0] = Color4F(0.5f, 0.3f, 0, 1); // brown
	colors[1] = Color4F(0, 0.5f, 0.1f, 1); // dark green
	colors[2] = Color4F(1, 1, 0, 1); // yellow
	colors[3] = Color4F(0, 0, 1, 1); // blue
	colors[4] = Color4F(1, 0, 1, 1); // magenta
	colors[5] = Color4F(1, 0, 0, 1); // red
	colors[6] = Color4F(0, 1, 0, 1); // light green     

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 10; j++) {
			circlesCoords[i][j][0] = (float)(((float)i - 3.25) + (float)(j % 2) / 2) * circleSize;
			circlesCoords[i][j][1] = ((float)j / 2 - 2) * (float)sqrt(3) * circleSize;
			circlesPresent[i][j] = false;
			circlesToFall[i][j] = false;
			circlesToVanish[i][j] = false;
			if (j != 9) {
				circleCreate(i, j);
			}
		}
	}

	vanishCheck();

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 8; j++) {
			if ((!(i % 2)) && (!(j % 2))) {
				pm[i][j][0][0] = i / 2;
				pm[i][j][0][1] = j;
				pm[i][j][1][0] = i / 2 + 1;
				pm[i][j][1][1] = j;
				pm[i][j][2][0] = i / 2;
				pm[i][j][2][1] = j + 1;
			}
			if ((i % 2) && (!(j % 2))) {
				pm[i][j][0][0] = i / 2 + 1;
				pm[i][j][0][1] = j;
				pm[i][j][1][0] = i / 2 + 1;
				pm[i][j][1][1] = j + 1;
				pm[i][j][2][0] = i / 2;
				pm[i][j][2][1] = j + 1;
			}
			if ((!(i % 2)) && (j % 2)) {
				pm[i][j][0][0] = i / 2;
				pm[i][j][0][1] = j;
				pm[i][j][1][0] = i / 2 + 1;
				pm[i][j][1][1] = j + 1;
				pm[i][j][2][0] = i / 2;
				pm[i][j][2][1] = j + 1;
			}
			if ((i % 2) && (j % 2)) {
				pm[i][j][0][0] = i / 2;
				pm[i][j][0][1] = j;
				pm[i][j][1][0] = i / 2 + 1;
				pm[i][j][1][1] = j;
				pm[i][j][2][0] = i / 2 + 1;
				pm[i][j][2][1] = j + 1;
			}
		}
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 8; j++) {
			cursorCoords[i][j][0] = (float)(i * 0.5 - 2.75) * circleSize;
			cursorCoords[i][j][1] = (float)(j * 3 - 11 + ((i + j) % 2)) * (float)sqrt(3) / 6 * circleSize;
		}
	}

	cursor = DrawNode::create();
	cursor->drawTriangle(Vec2((float)-0.5 * circleSize, (float)-sqrt(3) / 6 * circleSize),
		Vec2((float)0.5 * circleSize, (float)-sqrt(3) / 6 * circleSize),
		Vec2((float)0, (float)sqrt(3) / 3 * circleSize),
		Color4F(1, 1, 1, 1));
	cpx = 0;
	cpy = 0;
	cursor->setPositionX(cursorCoords[cpx][cpy][0]);
	cursor->setPositionY(cursorCoords[cpx][cpy][1]);
	this->addChild(cursor, 3);

	auto buttonSize = visibleSize.height / 3;

	m_rotateRightButton = ui::Button::create("rotate.png");
	m_rotateRightButton->setPosition(Vec2((-visibleSize.width + buttonSize) / 2, buttonSize));
	m_rotateRightButton->setScale(buttonSize / m_rotateRightButton->getContentSize().height);
	m_rotateRightButton->setScaleX(-m_rotateRightButton->getScaleX());
	m_rotateRightButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::rotateRightCallback, this));
	this->addChild(m_rotateRightButton);

	m_rotateLeftButton = ui::Button::create("rotate.png");
	m_rotateLeftButton->setPosition(Vec2((visibleSize.width - buttonSize) / 2, -buttonSize));
	m_rotateLeftButton->setScale(buttonSize / m_rotateLeftButton->getContentSize().height);
	m_rotateLeftButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::rotateLeftCallback, this));
	this->addChild(m_rotateLeftButton);

	m_moveRightButton = ui::Button::create("select.png");
	m_moveRightButton->setPosition(Vec2((visibleSize.width - buttonSize) / 2, 0));
	m_moveRightButton->setScale(buttonSize / m_moveRightButton->getContentSize().height);
	m_moveRightButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::moveRightCallback, this));
	this->addChild(m_moveRightButton);

	m_moveLeftButton = ui::Button::create("select.png");
	m_moveLeftButton->setPosition(Vec2((-visibleSize.width + buttonSize) / 2, 0));
	m_moveLeftButton->setScale(buttonSize / m_moveLeftButton->getContentSize().height);
	m_moveLeftButton->setRotation(180);
	m_moveLeftButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::moveLeftCallback, this));
	this->addChild(m_moveLeftButton);

	m_moveUpButton = ui::Button::create("select.png");
	m_moveUpButton->setPosition(Vec2((+visibleSize.width - buttonSize) / 2, buttonSize));
	m_moveUpButton->setScale(buttonSize / m_moveUpButton->getContentSize().height);
	m_moveUpButton->setRotation(270);
	m_moveUpButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::moveUpCallback, this));
	this->addChild(m_moveUpButton);

	m_moveDownButton = ui::Button::create("select.png");
	m_moveDownButton->setPosition(Vec2((-visibleSize.width + buttonSize) / 2, -buttonSize));
	m_moveDownButton->setScale(buttonSize / m_moveDownButton->getContentSize().height);
	m_moveDownButton->setRotation(90);
	m_moveDownButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::moveDownCallback, this));
	this->addChild(m_moveDownButton);

	return true;
}

void HelloWorld::update(float delta) {
	if (circlesVanishing) {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 9; j++) {
				if (circlesToVanish[i][j]) {
					auto temp = (circles[i][j]->getScaleX() - delta * 2);
					if (temp > 0) {
						circles[i][j]->setScaleX(temp);
						circles[i][j]->setScaleY(temp);
					}
					else {
						circlesToVanish[i][j] = false;
						circlesToFall[i][j] = true;
						circlesFalling = true;
						circlesVanishing = false;
						circlesPresent[i][j] = false;
						this->removeChild(circles[i][j]);
					}
				}
			}
		}
	}
	if (circlesFalling) {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 9; j++) {
				if (circlesToFall[i][j]) {
					if ((!circlesPresent[i][j + 1]) && j == 8) {
						circleCreate(i, j + 1);
					}
					if (circlesPresent[i][j + 1]) {
						auto temp = circles[i][j + 1]->getPositionY() + (circlesCoords[i][j][1] - circlesCoords[i][j + 1][1]) * delta * coef;
						if (temp > circlesCoords[i][j][1]) {
							circles[i][j + 1]->setPositionX(circles[i][j + 1]->getPositionX() + (circlesCoords[i][j][0] - circlesCoords[i][j + 1][0]) * delta * coef);
							circles[i][j + 1]->setPositionY(temp);
						}
						else {
							circles[i][j + 1]->setPositionX(circlesCoords[i][j][0]);
							circles[i][j + 1]->setPositionY(circlesCoords[i][j][1]);
							circles[i][j] = circles[i][j + 1];
							circlesColors[i][j] = circlesColors[i][j + 1];
							circlesPresent[i][j + 1] = false;
							circlesToFall[i][j + 1] = true;
							circlesPresent[i][j] = true;
							circlesToFall[i][j] = false;
						}
					}
				}
			}
		}
		circlesFalling = false;
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 9; j++) {
				if (circlesToFall[i][j]) {
					circlesFalling = true;
				}
			}
		}
		if (!circlesFalling) {
			vanishCheck();
		}
	}
	if (circlesTurn) {
		if (direction) {
			auto temp = circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->getPositionX() + delta * (circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]
				- circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][0]) * coef;
			if (temp < circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]) {
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionX(circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->getPositionX() + delta
					*(circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][0] - circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][0]) * coef);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionX(circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->getPositionX() + delta
					*(circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][0] - circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]) * coef);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionX(temp);
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionY(circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->getPositionY() + delta
					*(circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][1] - circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][1]) * coef);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionY(circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->getPositionY() + delta
					*(circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][1] - circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][1]) * coef);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionY(circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->getPositionY() + delta
					*(circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][1] - circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][1]) * coef);
			}
			else {
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionX(circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][0]);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionX(circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][0]);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionX(circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]);
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionY(circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][1]);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionY(circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][1]);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionY(circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][1]);

				auto tempCat = circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]];
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]] = circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]];
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]] = circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]];
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]] = tempCat;

				auto tempColor = circlesColors[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]];
				circlesColors[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]] = circlesColors[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]];
				circlesColors[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]] = circlesColors[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]];
				circlesColors[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]] = tempColor;

				circlesTurn = false;
				vanishCheck();
			}
		}
		else {
			auto temp = circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->getPositionX() + delta * (circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]
				- circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][0]) * coef;
			if (temp < circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]) {
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionX(temp);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionX(circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->getPositionX() + delta
					* (circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][0] - circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]) * coef);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionX(circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->getPositionX() + delta
					* (circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][0] - circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][0]) * coef);
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionY(circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->getPositionY() + delta
					* (circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][1] - circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][1]) * coef);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionY(circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->getPositionY() + delta
					* (circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][1] - circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][1]) * coef);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionY(circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->getPositionY() + delta
					* (circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][1] - circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][1]) * coef);
			}
			else {
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionX(circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][0]);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionX(circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][0]);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionX(circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][0]);
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]]->setPositionY(circlesCoords[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]][1]);
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]]->setPositionY(circlesCoords[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]][1]);
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]]->setPositionY(circlesCoords[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]][1]);

				auto tempCat = circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]];
				circles[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]] = circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]];
				circles[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]] = circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]];
				circles[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]] = tempCat;

				auto tempColor = circlesColors[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]];
				circlesColors[pm[cpx][cpy][0][0]][pm[cpx][cpy][0][1]] = circlesColors[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]];
				circlesColors[pm[cpx][cpy][2][0]][pm[cpx][cpy][2][1]] = circlesColors[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]];
				circlesColors[pm[cpx][cpy][1][0]][pm[cpx][cpy][1][1]] = tempColor;

				circlesTurn = false;
				vanishCheck();
			}
		}
	}
}

void HelloWorld::circleCreate(int i, int j) {
	int sectorsInCircle = (int)(circleSize / 3.5);
	circles[i][j] = DrawNode::create();
	circlesColors[i][j] = rand() % 7;
	circlesPresent[i][j] = true;
	for (int k = 0; k < sectorsInCircle; k++) {
		circles[i][j]->drawTriangle(Vec2(0, 0),
			Vec2((float)sin((2 * M_PI / sectorsInCircle) * k) / 2 * circleSize, (float)cos((2 * M_PI / sectorsInCircle) * k) / 2 * circleSize),
			Vec2((float)sin((2 * M_PI / sectorsInCircle) * (k - 1)) / 2 * circleSize, (float)cos((2 * M_PI / sectorsInCircle) * (k - 1)) / 2 * circleSize),
			colors[circlesColors[i][j]]);
	};
	circles[i][j]->setPositionX(circlesCoords[i][j][0]);
	circles[i][j]->setPositionY(circlesCoords[i][j][1]);
	circles[i][j]->setScaleX(1);
	circles[i][j]->setScaleY(1);
	addChild(circles[i][j], 2);
}

void HelloWorld::vanishCheck() {
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			if (i < 5 && circlesColors[i][j] == circlesColors[i + 1][j] && circlesColors[i][j] == circlesColors[i + 2][j]) {
				circlesToVanish[i][j] = true;
				circlesToVanish[i + 1][j] = true;
				circlesToVanish[i + 2][j] = true;
			}
			if (j < 7) {
				if (!(j % 2)) {
					if (i < 6 && circlesColors[i][j] == circlesColors[i][j + 1] && circlesColors[i][j] == circlesColors[i + 1][j + 2]) {
						circlesToVanish[i][j] = true;
						circlesToVanish[i][j + 1] = true;
						circlesToVanish[i + 1][j + 2] = true;
					}
					if (i > 0 && circlesColors[i][j] == circlesColors[i - 1][j + 1] && circlesColors[i][j] == circlesColors[i - 1][j + 2]) {
						circlesToVanish[i][j] = true;
						circlesToVanish[i - 1][j + 1] = true;
						circlesToVanish[i - 1][j + 2] = true;
					}
				}
				else {
					if (i < 6 && circlesColors[i][j] == circlesColors[i + 1][j + 1] && circlesColors[i][j] == circlesColors[i + 1][j + 2]) {
						circlesToVanish[i][j] = true;
						circlesToVanish[i + 1][j + 1] = true;
						circlesToVanish[i + 1][j + 2] = true;
					}
					if (i > 0 && circlesColors[i][j] == circlesColors[i][j + 1] && circlesColors[i][j] == circlesColors[i - 1][j + 2]) {
						circlesToVanish[i][j] = true;
						circlesToVanish[i][j + 1] = true;
						circlesToVanish[i - 1][j + 2] = true;
					}
				}
			}
			if (circlesToVanish[i][j]) {
				circlesVanishing = true;
			}
		}
	}
}

void HelloWorld::rotateRightCallback(Ref* pSender) {
	if ((!circlesTurn) && (!circlesVanishing) && (!circlesFalling)) {
		circlesTurn = true;
		direction = true;
	}
}

void HelloWorld::rotateLeftCallback(Ref* pSender) {
	if ((!circlesTurn) && (!circlesVanishing) && (!circlesFalling)) {
		circlesTurn = true;
		direction = false;
	}
}

void HelloWorld::moveRightCallback(Ref* pSender) {
	if (!circlesTurn) {
		if (cpx == 11) {
			cpx = 0;
		} else {
			++cpx;
		}
		commonTouch();
	}
}

void HelloWorld::moveLeftCallback(Ref* pSender) {
	if (!circlesTurn) {
		if (!cpx) {
			cpx = 11;
		} else {
			--cpx;
		}
		commonTouch();
	}
}

void HelloWorld::moveUpCallback(Ref* pSender) {
	if (!circlesTurn) {
		if (cpy == 7) {
			cpy = 0;
		} else {
			++cpy;
		}
		commonTouch();
	}
}

void HelloWorld::moveDownCallback(Ref* pSender) {
	if (!circlesTurn) {
		if (!cpy) {
			cpy = 7;
		} else {
			--cpy;
		}
		commonTouch();
	}
}

void HelloWorld::commonTouch() {
	cursor->setRotation(cursor->getRotation() + 60);
	cursor->setPositionX(cursorCoords[cpx][cpy][0]);
	cursor->setPositionY(cursorCoords[cpx][cpy][1]);
}