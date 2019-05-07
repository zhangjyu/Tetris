#include "lose.h"

Scene* Lose::createScene() {
	auto scene = Scene::create();
	auto layer = Lose::create();
	scene->addChild(layer);
	return scene;
}

bool Lose::init() {
	if (!Layer::init()) {
		return false;
	}
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto layer = LayerColor::create(Color4B(0, 128, 128, 255));
	layer->setContentSize(CCSizeMake(visibleSize.width, visibleSize.height));
	addChild(layer);
	//����ͼ
	auto sprite_back = Sprite::create("lose.png");
	sprite_back->setPosition(visibleSize.width / 2, visibleSize.height / 2);	//����λ��
	sprite_back->setAnchorPoint(Point(0.5, 0.5));	//����ê��
													//������������ʵ�ʴ�С
	Size size_back = sprite_back->getContentSize();	//��ȡ�����С(����ͼƬ��ʱ�򣬾����СΪͼƬ��С)
													//����ʵ�ʿ�Ⱥ��ܿ�ȣ������������ű���
	float scaleX = (float)visibleSize.width / (float)size_back.width;
	float scaleY = (float)visibleSize.height / (float)size_back.height;
	sprite_back->setScale(scaleX, scaleY);	//����

	addChild(sprite_back);
	//�˳���Ϸ��ť
	auto closeItem = MenuItemImage::create("CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Lose::onQuit, this));
	//����˵�����MenuItemSprite��ͼƬ�˵�����MenuItemImage������MenuItemImage�̳���MenuItemSprite������ͼƬ�˵�Ҳ���ھ���˵���
	//ÿ���ڵ㶼��һ����λ(ê)�㣬Ҳ������ͼ�����ĵ��Ӧ���������λ�á�Ĭ������£� anchorPoint ��������Ϊ��0.5,0.5��
	//��������غ���sprite.setScale(0.6f)�������ţ�sprite.setPosition���þ�������½�����
	closeItem->setAnchorPoint(Vec2(1, 1)); //�·���1,1�����£�0,0������

										   // create menu, it's an autorelease object
	auto mainMenu = Menu::create(closeItem, NULL);
	mainMenu->setAnchorPoint(Vec2(0, 0));
	mainMenu->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height - 20));
	this->addChild(mainMenu, 10);
	
	return true;
}
void Lose::onQuit(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}