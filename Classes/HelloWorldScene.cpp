#include "HelloWorldScene.h"

const std::string s_aTestPoses[] = {
    "Pos: Center",
    "Pos: Top",
    "Pos: TopLeft",
    "Pos: TopRight",
    "Pos: Bottom",
    "Pos: BottomLeft",
    "Pos: BottomRight",
};

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Plugin-x admob test", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    // ------------ AD test code ---------------
    
    _listener = new MyAdsListener();
    _admob = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AdsAdmob"));
    
    TAdsDeveloperInfo devInfo;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    devInfo["AdmobID"]      = "ca-app-pub-0000000000000000/0000000000";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    devInfo["AdmobID"]      = "ca-app-pub-0000000000000000/0000000000";
#endif
    
    _listener = new MyAdsListener();
    _admob = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsAdmob"));
    
    _admob->configDeveloperInfo(devInfo);
    _admob->setAdsListener(_listener);
    _admob->setDebugMode(true);
    
    Point posMid = Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    Point posBR = Point(origin.x + visibleSize.width, origin.y);
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemFont *pQuitItem = MenuItemFont::create("Quit", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    Size quitSize = pQuitItem->getContentSize();
    pQuitItem->setPosition(posBR + Point(- quitSize.width / 2, quitSize.height / 2));
    
    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pQuitItem, NULL);
    pMenu->setPosition( Point::ZERO );
    
    Label* label1 = Label::createWithTTF("ShowAds", "fonts/Marker Felt.ttf", 24);
    MenuItemLabel* pItemShow = MenuItemLabel::create(label1, CC_CALLBACK_1(HelloWorld::testShow, this));
    pItemShow->setAnchorPoint(Point(0.5f, 0));
    pMenu->addChild(pItemShow, 0);
    pItemShow->setPosition(posMid + Point(-100, -120));
    
    Label* label2 = Label::createWithTTF("HideAds", "fonts/Marker Felt.ttf", 24);
    MenuItemLabel* pItemHide = MenuItemLabel::create(label2, CC_CALLBACK_1(HelloWorld::testHide, this));
    pItemHide->setAnchorPoint(Point(0.5f, 0));
    pMenu->addChild(pItemHide, 0);
    pItemHide->setPosition(posMid + Point(100, -120));
    
    // poses item
    _posItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HelloWorld::posChanged, this),
                                                  MenuItemFont::create( s_aTestPoses[0].c_str() ),
                                                  NULL );
    int posLen = sizeof(s_aTestPoses) / sizeof(std::string);
    for (int i = 1; i < posLen; ++i)
    {
        _posItem->getSubItems().pushBack( MenuItemFont::create( s_aTestPoses[i].c_str() ) );
    }
    _posItem->setPosition(posMid + Point(0, 120));
    pMenu->addChild(_posItem);
    
    // init options
    _ads = _admob;
    _pos = ProtocolAds::kPosCenter;
    
    // init the AdsInfo
    adInfo["AdmobType"] = "1";
    //adInfo["AdmobSizeEnum"] = "1";          // 1 == kSizeBanner
    adInfo["AdmobSizeEnum"] = "6";          // 6 == kSizeSmartBanner
    
    this->addChild(pMenu, 1);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::testShow(Ref* pSender)
{
    if (_ads)
    {
        _ads->showAds(adInfo, _pos);
    }
}

void HelloWorld::testHide(Ref* pSender)
{
    _ads->hideAds(adInfo);
}

void HelloWorld::posChanged(Ref* pSender)
{
    int selectIndex = _posItem->getSelectedIndex();
    _pos = (ProtocolAds::AdsPos) selectIndex;
    log("pos selected change to : %d", _pos);
}

void MyAdsListener::onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg)
{
    log("OnAdsResult, code : %d, msg : %s", code, msg);
}

void MyAdsListener::onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points)
{
    log("Player get points : %d", points);
    
    // @warning should add code to give game-money to player here
    
    // spend the points of player
    if (pAdsPlugin != NULL) {
        pAdsPlugin->spendPoints(points);
    }
}