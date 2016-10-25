#include "Upgrade.h"
#include "HelloWorldScene.h"

#define DOWNLOAD_FIEL	 "download"	//下载后保存的文件夹名

Upgrade::Upgrade():
	_pathToSave(""),
	_showDownloadInfo(NULL)
{

}
Upgrade::~Upgrade()
{
	AssetsManager* assetManager = getAssetManager();
	CC_SAFE_DELETE(assetManager);
}
bool Upgrade::init()
{
	if(!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	initDownloadDir();
	_showDownloadInfo = Label::createWithSystemFont("", "Arial", 20);
	this->addChild(_showDownloadInfo);
	_showDownloadInfo->setPosition(Vec2(winSize.width / 2, winSize.height / 2 -20));

	auto itemLabel1 = MenuItemLabel::create(Label::createWithSystemFont("Reset", "Arail", 20),CC_CALLBACK_1(Upgrade::reset, this));
	auto itemLabel2 = MenuItemLabel::create(Label::createWithSystemFont("Upgrade", "Arail", 20), CC_CALLBACK_1(Upgrade::upgrade, this));
	auto itemLabel3 = MenuItemLabel::create(Label::createWithSystemFont("getClientVersion", "Arail", 20),CC_CALLBACK_1(Upgrade::getClientVersion, this));
	auto itemLabel4 = MenuItemLabel::create(Label::createWithSystemFont("checkUpdate", "Arail", 20), CC_CALLBACK_1(Upgrade::checkUpdate, this));
	auto itemLabel5 = MenuItemLabel::create(Label::createWithSystemFont("enterScene", "Arail", 20), CC_CALLBACK_1(Upgrade::enterScene, this));

	auto menu = Menu::create(itemLabel1, itemLabel2, itemLabel3, itemLabel4,itemLabel5, NULL);
	this->addChild(menu);

	itemLabel1->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 20));
	itemLabel2->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	itemLabel3->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 40));
	itemLabel4->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 60));
	itemLabel5->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 80));

	menu->setPosition(Vec2::ZERO);
	//getAssetManager()->update();
	return true;
}

void Upgrade::onError(AssetsManager::ErrorCode errorCode)
{
	if(errorCode == AssetsManager::ErrorCode::NO_NEW_VERSION)
	{
		_showDownloadInfo->setString("no new version");
	}
	else if (errorCode == AssetsManager::ErrorCode::NETWORK)
	{
		_showDownloadInfo->setString("network error");
	}
	else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE)
	{
		_showDownloadInfo->setString("create file error");
	}
}

void Upgrade::onProgress(int percent)
{
	if(percent < 0)
		return ;
	char progress[20];
	snprintf(progress, 20, "download %d%%", percent);
	_showDownloadInfo->setString(progress);
}
void Upgrade::onSuccess()
{
	log("download success");
	_showDownloadInfo->setString("download success");
	//std::string path = FileUtils::getInstance()->getWritablePath() + DOWNLOAD_FIEL;
}

AssetsManager* Upgrade::getAssetManager()
{
	static AssetsManager *assetManager = NULL;
	if(!assetManager)
	{
		assetManager = new AssetsManager("http://116.31.118.205:81/Resources.zip",
			"http://116.31.118.205:81/ResourcesVersion.txt",_pathToSave.c_str());
		assetManager->setDelegate(this);
		assetManager->setConnectionTimeout(8);
	}
	return assetManager;
}

void Upgrade::initDownloadDir()
{
	log("initDownLoadDir");
	_pathToSave = FileUtils::getInstance()->getWritablePath();
	_pathToSave += DOWNLOAD_FIEL;
	log("Path: %s ", _pathToSave.c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;
	if(!pDir)
	{
		mkdir(_pathToSave.c_str(), S_IRWXG | S_IRWXO);
	}
#else
	if((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_pathToSave.c_str(), 0);
	}
#endif

}

void Upgrade::reset(Ref* pSender)
{
	_showDownloadInfo->setString("");
	//Remove downloaded files
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	string command = "rm -r ";
	command += "\"" + _pathToSave + "\"";
	system(command.c_str());
#else
	std::string command = "rd /s /q ";
	command += "\"" + _pathToSave + "\"";
	system(command.c_str());
#endif
	getAssetManager()->deleteVersion();
	initDownloadDir();
}

void Upgrade::upgrade(Ref* pSender)
{
	_showDownloadInfo->setString("");
	getAssetManager()->update();
}

void  Upgrade::getClientVersion(Ref* pSender){
	CCString* msg = CCString::createWithFormat("current client version : %s", getAssetManager()->getVersion().c_str());
	log("%s",msg->getCString());
	_showDownloadInfo->setString(msg->getCString());
}

void Upgrade::checkUpdate(Ref* pSender){
	
	if (getAssetManager()->checkUpdate())
	{
		CCLOG("has new version");
		//log(getAssetManager()->getVersion2().c_str());
		_showDownloadInfo->setString("has new version");
	}else{
		CCLOG("has not new version");
		//log(getAssetManager()->getVersion2().c_str());
		_showDownloadInfo->setString("has not new version");
	}
}

void Upgrade::enterScene(Ref* pSender){
	CCDirector::sharedDirector()->replaceScene(HelloWorld::createScene());
}