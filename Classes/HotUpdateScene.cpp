#include "HotUpdateScene.h"
#include "cocostudio/CocoStudio.h"
//#include "LoadingUIScene.h"
//#include "../Data/Platform.h"


#define DOWNLOAD_FIEL	 "Resources"	//下载后保存的文件夹名

HotUpdateScene::HotUpdateScene():
	_pathToSave(""),
	Tip(NULL)
{

}
HotUpdateScene::~HotUpdateScene()
{
	AssetsManager* assetManager = getAssetManager();
	CC_SAFE_DELETE(assetManager);
}
Scene* HotUpdateScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HotUpdateScene::create();
	scene->addChild(layer);

	return scene;
}
bool HotUpdateScene::init()
{
	if(!Layer::init())
	{
		return false;
	}
	readJson();
	initView();
	initDownloadDir();
	checkUpdate();

	return true;
}
void HotUpdateScene::initView(){

	//Configuration::getInstance()->loadConfigFile("gameinfo.plist");
	auto m_res=CSLoader::createNode("HotUpdateScene.csb");
	if(m_res)
	{
		pLoadingbar=dynamic_cast<ui::LoadingBar*>(m_res->getChildByName("LoadingBar_1"));
		loadingbarBG=dynamic_cast<ui::ImageView*>(m_res->getChildByName("Image_4"));
		Tip=dynamic_cast<ui::Text*>(m_res->getChildByName("Text_1"));
		dialogBox = m_res->getChildByName("Dialog");
		//pLoadingbar->setVisible(false);
		lengthText=dynamic_cast<ui::Text*>(dialogBox->getChildByName("Text_3"));
		ui::Button* buttonOK = dynamic_cast<ui::Button*>(dialogBox->getChildByName("tipbutton1"));
		ui::Button* buttonCancel = dynamic_cast<ui::Button*>(dialogBox->getChildByName("tipbutton2"));
		buttonOK->addTouchEventListener(CC_CALLBACK_2(HotUpdateScene::touchEvent, this));
		buttonCancel->addTouchEventListener(CC_CALLBACK_2(HotUpdateScene::touchEvent, this));

		addChild(m_res,0,1);
	}

	return ;

}
void HotUpdateScene::touchEvent(Ref* pSender, ui::Widget::TouchEventType type)
{
	ui::Button* pBt=dynamic_cast<ui::Button*>(pSender);

	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:

		break;

	case ui::Widget::TouchEventType::MOVED:

		break;

	case ui::Widget::TouchEventType::ENDED:
		{
			if(pBt->getName() == "tipbutton1")
			{
				loadingbarBG->setVisible(true);
				Tip->setVisible(true);
				dialogBox->setVisible(false);
				update();
			}
			else if(pBt->getName() == "tipbutton2")
			{
				if(isImportant(_servicesVersion,getClientVersion()))
				{
#ifdef CC_TARGET_OS_IPHONE
					exit(0);
#else
					Director::getInstance()->end();
#endif
				}
				else
				{
					//auto loadingUIScene=LoadingUIScene::createScene();
					//Director::getInstance()->replaceScene(loadingUIScene);
				}
			}
		}
		break;
	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void HotUpdateScene::onError(AssetsManager::ErrorCode errorCode)
{
	if(errorCode == AssetsManager::ErrorCode::NO_NEW_VERSION)
	{
		Tip->setString("no new version");
		//auto loadingUIScene=LoadingUIScene::createScene();
		//Director::getInstance()->replaceScene(loadingUIScene);
	}
	else if (errorCode == AssetsManager::ErrorCode::NETWORK)
	{
		Tip->setString("network error");
		if(strcmp(ipAdrressBak.c_str(), "") == 0)
			return;
		ipAdrress = ipAdrressBak;
		ipAdrressBak = "";
		char packegeUrl[50] = {0};
		char versionFileUrl[50] = {0};
		sprintf(packegeUrl, "%s/Resources.zip", ipAdrress.c_str());
		sprintf(versionFileUrl, "%s/ResourcesVersion.txt", ipAdrress.c_str());
		getAssetManager()->reset(packegeUrl, versionFileUrl);
		checkUpdate();
	}
	else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE)
	{
		Tip->setString("create file error");
	}
}

void HotUpdateScene::onProgress(int percent)
{
	if(percent < 0)
		return ;
	char progress[50];
	//std::string tip=GETDATA_TIP_STRING("DownloadResources");
	snprintf(progress, 50, " %d%%", percent);
	Tip->setString(progress);
	pLoadingbar->setPercent(percent);
}
void HotUpdateScene::onSuccess()
{
	log("download success");
	Tip->setString("download success");
	//auto loadingUIScene=LoadingUIScene::createScene();
	//Director::getInstance()->replaceScene(loadingUIScene);
	//std::string path = FileUtils::getInstance()->getWritablePath() + DOWNLOAD_FIEL;
}

AssetsManager* HotUpdateScene::getAssetManager()
{
	static AssetsManager *assetManager = NULL;
	if(!assetManager)
	{
		char packegeUrl[50] = {0};
		char versionFileUrl[50] = {0};
		sprintf(packegeUrl, "%s/Resources.zip", ipAdrress.c_str());
		sprintf(versionFileUrl, "%s/ResourcesVersion.txt", ipAdrress.c_str());
		assetManager = new AssetsManager(packegeUrl, versionFileUrl,_pathToSave.c_str());
		assetManager->setDelegate(this);
		assetManager->setConnectionTimeout(8);
	}
	return assetManager;
}

void HotUpdateScene::initDownloadDir()
{
	log("initDownLoadDir");
	_pathToSave = FileUtils::getInstance()->getWritablePath();
	_pathToSave += DOWNLOAD_FIEL;
	log("Path: %s ", _pathToSave.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//DIR *pDir = NULL;
	//if(!pDir)
	//{
	//	mkdir(_pathToSave.c_str(), S_IRWXG | S_IRWXO);
	//}
	Platform pf;
	_pathToSave = pf.GetPackegPath();
#else
	if((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_pathToSave.c_str(), 0);
	}
#endif

}

std::string  HotUpdateScene::getClientVersion()
{
	return getAssetManager()->getVersion();
}

void HotUpdateScene::checkUpdate()
{

	if (getAssetManager()->checkUpdate())
	{
		CCLOG("has new version");
		Tip->setString("has new version");

		Tip->setVisible(false);
		getAssetManager()->loadURLHead();
		//while(!getAssetManager()->getDownloadFlag())
		//{
		//	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//		//Sleep(100);
		//	#else
		//		Sleep(100);
		//	#endif
		//}
		_downloadLength = getAssetManager()->getDownloadLength();
		char text[20] = {0};
		sprintf(text, "%.2f KB", _downloadLength / 1024);
		lengthText->setString(text);
		dialogBox->setVisible(true);
		_servicesVersion = getAssetManager()->getVersion2();

	}else{
		CCLOG("has not new version");
		Tip->setString("has not new version");

	}

}

void HotUpdateScene::update()
{
	Tip->setString("");
	getAssetManager()->update();
}

bool HotUpdateScene::isImportant(std::string version1, std::string version2)
{
	if(0 == strncmp(version1.c_str(), version2.c_str(), 1))
		return false;

	return false;
}

bool HotUpdateScene::readJson()
{
	std::string path = "";
	path.append("gameVersion.manifest");

	//*** 读取 json 文件 ***
	rapidjson::Document readdoc;
	bool bRet = false;
	ssize_t size = 0;
	std::string load_str;

	// getFileData 如果不指定，读取根目录是 Resources 文件夹
	unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
	load_str = std::string((const char*)titlech,size);

	//load_str = cocos2d::FileUtils::getInstance()->getStringFromFile("..\\myhero.json");
	readdoc.Parse<0>(load_str.c_str());	
	if(readdoc.HasParseError())
	{
		CCLOG("GetParseError%s\n", readdoc.GetParseError());
	}

	if(!readdoc.IsObject())
		return 0;

	rapidjson::Value& _json = readdoc["remoteManifestUrl"];
	const char* ch1 = _json.GetString();
	_json = readdoc["remoteManifestUrl_2"];
	const char* ch2 = _json.GetString();


	char *p; 
	p = strtok((char*)ch1, "/");
	ipAdrress.append(p);
	ipAdrress.append("//");
	p = strtok(NULL, "/");
	ipAdrress.append(p);

	p = strtok((char*)ch2, "/");
	ipAdrressBak.append(p);
	ipAdrressBak.append("//");
	p = strtok(NULL, "/");
	ipAdrressBak.append(p);


	return true;
}
