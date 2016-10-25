#include "UpdateEx.h"


bool UpdateEx::init()
{
	if (!Layer::init())
    {
        return false;
    }
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_pProcessLabel = Label::createWithSystemFont("", "Arial", 20);
	this->addChild(m_pProcessLabel);
	m_pProcessLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 -20));

	auto itemLabel1 = MenuItemLabel::create(Label::createWithSystemFont("Update", "Arail", 20),CC_CALLBACK_1(UpdateEx::update, this));

	auto menu = Menu::create(itemLabel1, NULL);
	this->addChild(menu);

	itemLabel1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 20));

	menu->setPosition(Vec2::ZERO);
	return true;
}

AssetsManagerEx* UpdateEx::getAssetsManagerEx()
{  
    static AssetsManagerEx *amEx = nullptr;  
    if (!amEx)  
    {  
        amEx = AssetsManagerEx::create("project_test.manifest",m_pathToSaveEx);  
        amEx->retain();  
  
  
        if (amEx->getLocalManifest()->isLoaded())  
        {  
            log("Fail to update assets, step skipped.");  
        }  
  
  
        auto listener = EventListenerAssetsManagerEx::create(amEx,CC_CALLBACK_1(UpdateEx::onAssetsEvent,this));  
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);  
    }  
    return amEx;  
}

void UpdateEx::InitDownLoadDirEx()
{  
    m_pathToSaveEx = FileUtils::getInstance()->getWritablePath() + "Update";  
  
  
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)  
    DIR *pDir = NULL;  
    pDir = opendir(m_pathToSaveEx.c_str());  
    if (!pDir)  
    {  
        mkdir(m_pathToSaveEx.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);  
    }  
#else  
    if ((GetFileAttributesA(m_pathToSaveEx.c_str())) == INVALID_FILE_ATTRIBUTES)  
    {  
        CreateDirectoryA(m_pathToSaveEx.c_str(), 0);  
    }  
#endif  
    CCLOG("InitDownLoadDirEx end");  
} 

void UpdateEx::onAssetsEvent(EventAssetsManagerEx* pEvent)  
{  
    switch (pEvent->getEventCode())  
    {  
    case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:  
        {  
            log("没有本地清单文件");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:  
        {  
            log("下载远程清单文件失败");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:  
        {  
            log("解析清单文件失败");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND:  
        {  
            log("找到新版本");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:  
        {  
            log("更新完成");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:  
        {  
            log("更新中");  
            char szBuf[256] = "";  
            sprintf(szBuf,"%s,%0.2f,%0.2f",pEvent->getAssetId().c_str(),pEvent->getPercentByFile(),pEvent->getPercent());  
            m_pProcessLabel->setString(szBuf);  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::ASSET_UPDATED:  
        {  
            log("检测更新");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::ERROR_UPDATING:  
        {  
            log("更新出错");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:  
        {  
            log("更新完成");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::UPDATE_FAILED:  
        {  
            log("更新失败");  
        }  
        break;  
    case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:  
        {  
            log("解压出错");  
        }  
        break;  
    default:  
        break;  
    }  
}

void UpdateEx::update(Ref* pSender)
{
	InitDownLoadDirEx();  
	getAssetsManagerEx()->update();  
}
