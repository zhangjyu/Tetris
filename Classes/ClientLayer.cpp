#include "ClientLayer.h"
#include "MScen.h"
#include "cocos2d.h"

#include "win.h"

Scene* ClientLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = ClientLayer::create();

	scene->addChild(layer);

	return scene;
}

bool ClientLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_client = SocketClient::construct();
	_client->onRecv = CC_CALLBACK_2(ClientLayer::onRecv, this);
	_client->onDisconnect = CC_CALLBACK_0(ClientLayer::onDisconnect, this);

	_server = SocketServer::getInstance();
	_server->startServer(7000);
	//ClientLayer::initNetwork();
	auto winSize = Director::getInstance()->getVisibleSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	TextFieldTTF *tf = TextFieldTTF::textFieldWithPlaceHolder("Please Click Input:", "Arial", 20);
	tf->setPosition(winSize.width / 2 - 50, winSize.height / 2 - 50);
	addChild(tf);
	//����
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [tf](Touch *t, Event *event) {
		if (tf->getBoundingBox().containsPoint(t->getLocation())) {
			tf->attachWithIME();
		}
		else {
			tf->detachWithIME();
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, tf);

	//auto myMenu = Menu::create();
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_0(ClientLayer::aaa, this, tf));

	cocos2d::Menu* m = Menu::create(closeItem, NULL);
	m->setPosition(winSize.width / 2 + 100, winSize.height / 2 - 50);
	this->addChild(m);
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_client->recv_brocast();
	mscen = MScen::create();
	mscen->endCallback(std::bind(&ClientLayer::sendend, this));
	mscen->checkCallback(std::bind(&ClientLayer::sendline, this));
	addChild(mscen);
#endif

	
	
	this->scheduleUpdate();
	log("client");
	return true;
}
	
void ClientLayer::initNetwork()
{
	
	
	int l = 5;
	log("_client->recv");
	l=_client->recv_brocast();
	//std::string str = _client->recv_brocast();
	log("_client->recv");
	std::string str = StringUtils::format("%d", l);
	auto winSize = Director::getInstance()->getVisibleSize();
	auto lblServer = Label::createWithSystemFont(str, "Arial", 36);
	lblServer->setPosition(Vec2(winSize.width * 0.4f, winSize.height * 0.4f));
	addChild(lblServer);
}
	
void ClientLayer::update(float dt) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (start == 0) {
		int data = 56;
		_server->sendMessage((const char*)&data, sizeof(int));
	}
#endif
	static float curTime = 0;
	curTime += dt;
	if (curTime > dt*2.f) {
		//int a=6;
		//_server->sendMessage((const char*)&a, sizeof(int));
		curTime -= dt*2.f;
	}
}

void ClientLayer::onEnter()
{
	Layer::onEnter();
}

void ClientLayer::onExit()
{
	_client->destroy();
	_client = nullptr;
	_server->destroyInstance();
	//Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
	//Layer::onExit();
}



void ClientLayer::onRecv(const char* data, int count)
{
	int a = 0;
	memcpy(&a, data, count);
	if (a == 404) {
		log("%d", a);
		//onExit();
		auto wi = Win::create();
		this->addChild(wi);
		onDisconnect();
		//_server->onDisconnect;
		//auto scene = Win::createScene();
		//Director::getInstance()->replaceScene(scene);
		//Layer::onExit();
	}
	if (a == 111) {
		mscen->upline();
	}
	if (a == 56) {
		start = 1;
	}
	log("%d", a);
}

void ClientLayer::onDisconnect()
{
	_server->~SocketServer();
	//_lblInfo->setString("Client disconnect");
}

void ClientLayer::sendend() {
	int data = 404;
	_server->sendMessage((const char*)&data, sizeof(int));
	//onExit();
	log("bbb");
	onDisconnect(); log("aaa");
	//_server->onDisconnect;
	//auto scene = Lose::createScene();
	//Director::getInstance()->replaceScene(scene);
	//Layer::onExit();
}
void ClientLayer::sendline() {
	int data = 111;
	_server->sendMessage((const char*)&data, sizeof(int));
}

/*void ClientLayer::getLocalIP(char* ip) {
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		log("0");
	}
	char hostname[256];
	ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR) {
		log("1");
	}
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL) {
		log("2");
	}
	strcpy(ip, inet_ntoa(*(in_addr*)*host->h_addr_list));
	log("%s", ip);
}*/
void ClientLayer::aaa(TextFieldTTF *tf) {
	auto label = Label::createWithTTF("label test", "fonts/Marker Felt.ttf", 32);
	label->setString(tf->getString());
	auto winSize = Director::getInstance()->getVisibleSize();
	label->setPosition(winSize.width / 2 + 200, winSize.height / 2 - 50);
	//addChild(label);
	std::string str = label->getString();
	std::string sss = "172.16.18.4";
	mscen = MScen::create();
	s=str.c_str();
	_client->connectServer(s, 8888);
	mscen->endCallback(std::bind(&ClientLayer::sendend, this));
	mscen->checkCallback(std::bind(&ClientLayer::sendline, this));
	addChild(mscen);
}
