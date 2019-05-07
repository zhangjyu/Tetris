#include "Djscene.h"
#include "djplay.h"

#include "lose.h"
using namespace cocos2d;

Scene* Djscene::createScene()
{
	auto scene = Scene::create();
	auto Layer = Djscene::create();

	scene->addChild(Layer);
	return scene;
}
bool Djscene::init() {
	if (!Layer::init()) {
		return false;
	}
	log("safdojdsaigjisdofjisdo");
	// ԭ���λ��
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// ��Ļ�Ŀɼ�����ֵ
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//�˳���Ϸ��ť
	auto closeItem = MenuItemImage::create("CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Djscene::onQuit, this));
	//����˵�����MenuItemSprite��ͼƬ�˵�����MenuItemImage������MenuItemImage�̳���MenuItemSprite������ͼƬ�˵�Ҳ���ھ���˵���
	//ÿ���ڵ㶼��һ����λ(ê)�㣬Ҳ������ͼ�����ĵ��Ӧ���������λ�á�Ĭ������£� anchorPoint ��������Ϊ��0.5,0.5��
	//��������غ���sprite.setScale(0.6f)�������ţ�sprite.setPosition���þ�������½�����
	closeItem->setAnchorPoint(Vec2(1, 1)); //�·���1,1�����£�0,0������

										   // create menu, it's an autorelease object
	auto mainMenu = Menu::create(closeItem, NULL);
	mainMenu->setAnchorPoint(Vec2(0, 0));
	mainMenu->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height - 20));
	this->addChild(mainMenu, 10);
	//����ͼ
	auto sprite_back = Sprite::create("b.png");
	sprite_back->setPosition(visibleSize.width / 2, visibleSize.height / 2);	//����λ��
	sprite_back->setAnchorPoint(Point(0.5, 0.5));	//����ê��
													//������������ʵ�ʴ�С
	Size size_back = sprite_back->getContentSize();	//��ȡ�����С(����ͼƬ��ʱ�򣬾����СΪͼƬ��С)
													//����ʵ�ʿ�Ⱥ��ܿ�ȣ������������ű���
	float scaleX = (float)visibleSize.width / (float)size_back.width;
	float scaleY = (float)visibleSize.height / (float)size_back.height;
	sprite_back->setScale(scaleX, scaleY);	//����

	addChild(sprite_back);

	djplay = Djplay::create();
	log("1");
	djplay->setPosition(Vec2(10, 10));
	djplay->setScoreCallback(std::bind(&Djscene::onGetScore, this, std::placeholders::_1));
	this->addChild(djplay);
	//������ͼƬ��ť
	auto rotateMenuItem = MenuItemImage::create("rotate.png", "rotate.png", std::bind(&Djscene::onRotateShape, this));
	auto downMenuItem = MenuItemImage::create("down.png", "down.png", std::bind(&Djscene::onMoveDownShape, this));
	auto leftMenuItem = MenuItemImage::create("left.png", "left.png", std::bind(&Djscene::onMoveLeftShape, this));
	auto rightMenuItem = MenuItemImage::create("right.png", "right.png", std::bind(&Djscene::onMoveRightShape, this));
	//���ð�ť��λ�ã������λ�ã����滹�в˵���λ��
	rotateMenuItem->setPosition(Vec2(0, 70));
	downMenuItem->setPosition(Vec2(0, -70));
	leftMenuItem->setPosition(Vec2(-90, 0));
	rightMenuItem->setPosition(Vec2(90, 0));
	auto menu = Menu::create(rotateMenuItem, downMenuItem, leftMenuItem, rightMenuItem, nullptr); //һ���Ե���Ӷ���˵���ť��һ���˵���
	menu->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + 200));  //���ò˵���λ��
	this->addChild(menu); //���˰����뵽������

						  //��ʾ�����ı�ǩ
						  //�����ʵ���ʵ������xml�ļ����ص��ʵ���
	auto *chnStrings = Dictionary::createWithContentsOfFile("CHN_Strings.xml");
	//ͨ��xml�ļ��е�key��ȡvalue
	const char *strScore = ((String*)chnStrings->objectForKey("Score"))->getCString();
	_scoreLabel = Label::createWithTTF(StringUtils::format("%s%d", strScore, 0), "AdobeHeitiStd-Regular.otf", 40);
	_scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - 130, origin.y + visibleSize.height - 50));
	_scoreLabel->setAnchorPoint(Vec2(0, 0.5));
	this->addChild(_scoreLabel);
	return true;
}


void Djscene::onRotateShape()
{
	djplay->rotateShape();
}

void Djscene::onMoveLeftShape()
{
	djplay->moveShapeLeft();
}

void Djscene::onMoveRightShape()
{
	djplay->moveShapeRight();
}

void Djscene::onMoveDownShape()
{
	djplay->moveShapeDown();
}

void Djscene::onQuit(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void Djscene::onGetScore(std::string score)
{
	if (score == "") return;
	log("onGetScore %s", score.c_str());
	_scoreLabel->setString(score);
}

void Djscene::onGameOver()
{
	//_callendFunc();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//�����ʵ���ʵ������xml�ļ����ص��ʵ���
	auto *chnStrings = Dictionary::createWithContentsOfFile("CHN_Strings.xml");
	//ͨ��xml�ļ��е�key��ȡvalue
	const char *strGameOver = ((String*)chnStrings->objectForKey("Score"))->getCString();
	auto label = Label::createWithTTF(strGameOver, "AdobeHeitiStd-Regular.otf", 60);
	label->setColor(Color3B(255, 0, 0));
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(label);
	auto lo = Lose::create();
	this->addChild(lo);
}
