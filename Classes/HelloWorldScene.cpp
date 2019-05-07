#include "HelloWorldScene.h"
#include "SelectLayer.h"
#include "Djscene.h"
#include "cocos2d.h"

USING_NS_CC;   //�����ռ�

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
	scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
	if ( !Layer::init() )
    {
        return false;
    }
	
	//����
	auto winSize = Director::getInstance()->getVisibleSize();
	auto sprite_back = Sprite::create("main.png");
	sprite_back->setPosition(winSize.width / 2, winSize.height / 2);	//����λ��
	sprite_back->setAnchorPoint(Point(0.5, 0.5));	//����ê��
													//������������ʵ�ʴ�С
	Size size_back = sprite_back->getContentSize();	//��ȡ�����С(����ͼƬ��ʱ�򣬾����СΪͼƬ��С)
													//����ʵ�ʿ�Ⱥ��ܿ�ȣ������������ű���
	float scaleX = (float)winSize.width / (float)size_back.width;
	float scaleY = (float)winSize.height / (float)size_back.height;
	sprite_back->setScale(scaleX, scaleY);	//����

	addChild(sprite_back);
	
	//������Ϸ
	auto label = Label::createWithSystemFont("Single player", "Arial", 36);
	auto menuplay = MenuItemLabel::create(label, CC_CALLBACK_1(HelloWorld::menudjplay, this));
	menuplay->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.6));
	//��������վ
	auto  label2 = Label::createWithSystemFont("LAN player", "Arial", 36);
	auto menuplay2 = MenuItemLabel::create(label2, CC_CALLBACK_1(HelloWorld::menujyplay, this));
	menuplay2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5));

	auto menu = Menu::create(menuplay, menuplay2, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);


	
    return true;
}
/*
char *HelloWorld::FontToUTF8(const char* font) {
	int len = MultiByteToWideChar(CP_ACP, 0, font, -1, NULL, 0);
	wchar_t *wstr = new wchar_t[len+1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, font, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char *str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr)delete[] wstr;
	return str;
}*/
void HelloWorld::menudjplay(Ref* sender) {
	auto scene = Djscene::createScene();
	Director::getInstance()->replaceScene(scene);
}
void HelloWorld::menujyplay(Ref* sender) {
	auto scene = SelectLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}
/*
void HelloWorld::aaa(TextFieldTTF *tf) {
	auto label = Label::createWithTTF("label test", "fonts/Marker Felt.ttf", 32);
	label->setString(tf->getString());
	auto winSize = Director::getInstance()->getVisibleSize();
	label->setPosition(winSize.width / 2 + 200, winSize.height / 2 - 50);
	addChild(label);
	std::string str = label->getString();
}*/