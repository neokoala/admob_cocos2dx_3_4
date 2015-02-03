#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginManager.h"
#include "ProtocolAds.h"

USING_NS_CC;
using namespace cocos2d::plugin;

class MyAdsListener : public AdsListener
{
public:
    virtual void onAdsResult(AdsResultCode code, const char* msg);
	virtual void onPlayerGetPoints(ProtocolAds* pAdsPlugin, int points);
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    // a selector callback
    void menuBackCallback(Ref* pSender);
    void testShow(Ref* pSender);
    void testHide(Ref* pSender);
    
    // option changed callback
    void caseChanged(Ref* pSender);
    void posChanged(Ref* pSender);
    void typeChanged(Ref* pSender);
    
private:
    ProtocolAds* _admob;
    
    MyAdsListener* _listener;
    
    MenuItemToggle* _caseItem;
    MenuItemToggle* _posItem;
    MenuItemToggle* _typeItem;
    
    ProtocolAds* _ads;
    ProtocolAds::AdsPos _pos;
    
    TAdsInfo adInfo;
};

#endif // __HELLOWORLD_SCENE_H__
