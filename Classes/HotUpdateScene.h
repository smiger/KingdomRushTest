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
	void checkUpdate();			//���汾
	std::string getClientVersion();	//��ȡ���ذ汾��
	void update();				//������Դ

	virtual void onError(extension::AssetsManager::ErrorCode errorCode); //������Ϣ
	virtual void onProgress(int percent);	//�������ؽ���
	virtual void onSuccess();				//���سɹ�

	AssetsManager* getAssetManager();
	void initDownloadDir();					//��������Ŀ¼
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