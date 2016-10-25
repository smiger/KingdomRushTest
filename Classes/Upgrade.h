#pragma

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Upgrade : public Layer, public extension::AssetsManagerDelegateProtocol
{
public:
	Upgrade();
	virtual ~Upgrade();
	virtual bool init();
	void upgrade(Ref* pSender); //检查版本更新
	void reset(Ref* pSender);	//重置版本
	void getClientVersion(Ref* pSender);
	void checkUpdate(Ref* pSender);
	void enterScene(Ref* pSender);

	virtual void onError(extension::AssetsManager::ErrorCode errorCode); //错误信息
	virtual void onProgress(int percent); //更新下载进度
	virtual void onSuccess(); //下载成功
	CREATE_FUNC(Upgrade);
private:
	AssetsManager* getAssetManager();
	void initDownloadDir();	//创建下载目录
private:
	std::string _pathToSave;
	Label *_showDownloadInfo;

};