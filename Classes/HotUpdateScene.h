#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HotUpdateScene : public Layer, public extension::AssetsManagerDelegateProtocol
{
public:
	HotUpdateScene();
	virtual ~HotUpdateScene();
	virtual bool init();
	CREATE_FUNC(HotUpdateScene);
	static Scene* createScene();
	void initView();
	void touchEvent(Ref* pSender, ui::Widget::TouchEventType type);
	void checkUpdate();			//检查版本
	std::string getClientVersion();	//获取本地版本号
	void update();				//更新资源

	virtual void onError(extension::AssetsManager::ErrorCode errorCode); //错误信息
	virtual void onProgress(int percent);	//更新下载进度
	virtual void onSuccess();				//下载成功

	AssetsManager* getAssetManager();
	void initDownloadDir();					//创建下载目录
	bool isImportant(std::string version1, std::string version2);

	bool readJson();

private:
	ui::LoadingBar *pLoadingbar;
	ui::ImageView *loadingbarBG;
	ui::Text *Tip;
	ui::Text *lengthText;
	Node* dialogBox;
	std::string _pathToSave;
	std::string _servicesVersion;
	double _downloadLength;

	std::string ipAdrress;
	std::string ipAdrressBak;
};