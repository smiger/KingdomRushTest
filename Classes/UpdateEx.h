#pragma

#include "cocos2d.h"
#include "extensions\cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class UpdateEx : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(UpdateEx);

	AssetsManagerEx* getAssetsManagerEx();
	void InitDownLoadDirEx();
	void onAssetsEvent(EventAssetsManagerEx* pEvent);
	void update(Ref* pSender);

private:
	std::string m_pathToSaveEx;
	Label *m_pProcessLabel;


};