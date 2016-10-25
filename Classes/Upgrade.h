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
	void upgrade(Ref* pSender); //���汾����
	void reset(Ref* pSender);	//���ð汾
	void getClientVersion(Ref* pSender);
	void checkUpdate(Ref* pSender);
	void enterScene(Ref* pSender);

	virtual void onError(extension::AssetsManager::ErrorCode errorCode); //������Ϣ
	virtual void onProgress(int percent); //�������ؽ���
	virtual void onSuccess(); //���سɹ�
	CREATE_FUNC(Upgrade);
private:
	AssetsManager* getAssetManager();
	void initDownloadDir();	//��������Ŀ¼
private:
	std::string _pathToSave;
	Label *_showDownloadInfo;

};