#include "ServerLayer.h"
#include "socket/SocketServer.h"
#include "MScen.h"

#include "socket/SocketClient.h"

#include "TetrisLayer.h"
#include "win.h"

Scene* ServerLayer::createScene() {
	auto scene = Scene::create();
	auto layer = ServerLayer::create();
	scene->addChild(layer);
	return scene;
}

bool ServerLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	_server = SocketServer::getInstance();
	_server->startServer(8888);
	//ServerLayer::initNetwork();
	

	int l = 5;
	_server->send_brocast();//windows发送广播,android获取ip
	std::string str = StringUtils::format("%d", l);

	auto winSize = Director::getInstance()->getVisibleSize();
	auto lblServer = Label::createWithSystemFont(str, "Arial", 36);
	lblServer->setPosition(Vec2(winSize.width * 0.4f, winSize.height * 0.4f));
	//addChild(lblServer);
	
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	auto al = Label::createWithSystemFont(_server->ipp, "Arial", 36);
	al->setPosition(Vec2(winSize.width * 0.4f, winSize.height * 0.4f));
	addChild(al);
#endif
	_client = SocketClient::construct();
	_client->onRecv = CC_CALLBACK_2(ServerLayer::onRecv, this);
	_client->onDisconnect = CC_CALLBACK_0(ServerLayer::onDisconnect, this);
	std::thread th(&ServerLayer::sock, this);
	th.detach();
	//sock();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	mscen = MScen::create();
	mscen->endCallback(std::bind(&ServerLayer::sendend, this));
	mscen->checkCallback(std::bind(&ServerLayer::sendline, this));
	addChild(mscen);
#endif
	log("??????????????");
	this->scheduleUpdate();
	
	log("server");
	return true;
}
void ServerLayer::initNetwork() {
	_server = SocketServer::getInstance();
	_server->startServer(8888);
	log("***************************");
	
}
/*
void ServerLayer::sendData(DataType type) {
	GameData data;
	data.dataType = type;
	data.dataSize = sizeof(GameData);
	data.position = _enemy->getPosition();
	int data=4;
	_server->sendMessage((const char*)&data, sizeof(int));
}*/
void ServerLayer::onEnter() {
	Layer::onEnter();
}

void ServerLayer::onDisconnect() {
	_server->~SocketServer();
}
void ServerLayer::onExit() {
	_server->destroyInstance();
	_client->destroy();
	log("sadaffdafdssafdaf");
	//Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
	//Layer::onExit();
}

void ServerLayer::update(float dt) {
	static float curTime = 0;
	curTime += dt;
	if (curTime > dt*2.f) {
		//int data = 4;
		//_server->sendMessage((const char*)&data, sizeof(int));
		curTime -= dt*2.f;
	}
}

void ServerLayer::onRecv(const char* data, int count) {
	int a = 0;
	memcpy(&a, data, count);
	if (a == 404) {
		log("%d", a);
		auto wi = Win::create();
		this->addChild(wi);
		
		onDisconnect();
	}
	if (a == 111) {
		mscen->upline();
	}
	log("%d", a);

	if (a == 56&&start==0) {
		int data = 56;
		_server->sendMessage((const char*)&data, sizeof(int));
		mscen = MScen::create();
		mscen->endCallback(std::bind(&ServerLayer::sendend, this));
		mscen->checkCallback(std::bind(&ServerLayer::sendline, this));
		addChild(mscen);
		
		start = 1;
	}
}
void ServerLayer::sendend() {
	int data = 404;
	_server->sendMessage((const char*)&data, sizeof(int));
	onDisconnect();
}
void ServerLayer::sendline() {
	int data = 111;
	_server->sendMessage((const char*)&data, sizeof(int));
}
void ServerLayer::sock() {
	while (_server->stauts == 0) {}
	_client->connectServer(_server->ip, 7000);
	//int data = 56;
	//_server->sendMessage((const char*)&data, sizeof(int));
	
}