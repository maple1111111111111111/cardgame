#include "CardView.h"
#include <iostream>
using namespace cocos2d;

//就是CREATE_FUNC宏的使用 
CardView* CardView::create(const Card& card) {
    CardView* pRet = new(std::nothrow) CardView(card);  // 使用带参构造函数
    if (pRet && pRet->init()) {  // 调用无参init
        pRet->autorelease();
        return pRet;
    }
    else {
        std::cout<< "create card view failed" << std::endl;
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

CardView::CardView(const Card& card) : _card(card) {}

bool CardView::init() {
    if (!Sprite::init()) return false;

    // ========= 基础：底图 =========
    auto baseSprite = Sprite::create("card_general.png");
    if (!baseSprite) { CCLOG("Error: 找不到 card_general.png"); return false; }
    this->setContentSize(baseSprite->getContentSize());
    baseSprite->setPosition(this->getContentSize() / 2);
    this->addChild(baseSprite, 0);

    // ========= 花色与红黑判定 =========
    std::string suitName = _card.getSuitName();      // 可能返回 "Heart"/"heart" 等
    std::string suitLower = suitName;
    std::transform(suitLower.begin(), suitLower.end(), suitLower.begin(), ::tolower);

    const bool isRed = (suitLower == "heart" || suitLower == "diamond");

    // ========= 大号数字（随花色同色） =========
    std::string numberName = _card.getNumberName();  // "A","2","10","J","Q","K"
    bool bigFromColored = false;
    Sprite* numberSprite = nullptr;
    {
        // 先尝试 big_red/big_black 两套资源
        std::string bigTry = std::string("number/") + (isRed ? "big_red_" : "big_black_") + numberName + ".png";
        numberSprite = Sprite::create(bigTry);
        bigFromColored = (numberSprite != nullptr);

        // 若红图缺失，回退黑图
        if (!numberSprite && isRed) {
            bigTry = "number/big_black_" + numberName + ".png";
            numberSprite = Sprite::create(bigTry);
        }
        // 最后兜底
        if (!numberSprite) numberSprite = Sprite::create("number/default.png");
        if (!numberSprite) return false;

        // 若没拿到彩色版，则给黑图着红色
        if (isRed && !bigFromColored) {
            numberSprite->setColor(Color3B(220, 40, 50)); // 红
        }
        else if (!isRed && !bigFromColored) {
            numberSprite->setColor(Color3B(25, 35, 40));  // 黑
        }

        numberSprite->setPosition(this->getContentSize().width * 0.5f,
            this->getContentSize().height * 0.5f);
        this->addChild(numberSprite, 1);
    }

    // ========= 右上角 suit（随花色同色） =========
    Sprite* suitSprite = nullptr;
    {
        bool suitFromColored = false;
        std::string dirColored = std::string("suits/") + (isRed ? "red/" : "black/");
        // 1) 优先彩色目录（原大小写名、再小写名）
        suitSprite = Sprite::create(dirColored + suitName + ".png");
        if (!suitSprite) suitSprite = Sprite::create(dirColored + suitLower + ".png");
        suitFromColored = (suitSprite != nullptr);
        // 2) 回退旧路径
        if (!suitSprite) suitSprite = Sprite::create("suits/" + suitName + ".png");
        if (!suitSprite) suitSprite = Sprite::create("suits/" + suitLower + ".png");
        // 3) 最后兜底
        if (!suitSprite) suitSprite = Sprite::create("suits/default.png");
        if (!suitSprite) return false;

        // 
        if (isRed && !suitFromColored) {
            suitSprite->setColor(Color3B(220, 40, 50));
        }
        else if (!isRed && !suitFromColored) {
            suitSprite->setColor(Color3B(25, 35, 40));
        }

        const float kSuitToCardH = 0.18f;
        float targetH = this->getContentSize().height * kSuitToCardH;
        float scale = targetH / suitSprite->getContentSize().height;
        suitSprite->setScale(scale);
        suitSprite->setAnchorPoint({ 1.f, 1.f });
        const float kPadding = 10.f;
        suitSprite->setPosition(this->getContentSize().width - kPadding,
            this->getContentSize().height - kPadding);
        suitSprite->setLocalZOrder(2);
        this->addChild(suitSprite, 2);
    }

    // ========= 左上角小号数字（随花色同色） =========
    {
        bool smallFromColored = false;
        std::string smallTry = std::string("number/") + (isRed ? "small_red_" : "small_black_") + numberName + ".png";
        auto smallNum = Sprite::create(smallTry);
        smallFromColored = (smallNum != nullptr);

        // 若红小号缺失，回退黑小号
        if (!smallNum && isRed) {
            smallTry = "number/small_black_" + numberName + ".png";
            smallNum = Sprite::create(smallTry);
        }
        // 兜底
        if (!smallNum) smallNum = Sprite::create("number/default_small.png");
        if (smallNum) {
            if (isRed && !smallFromColored) {
                smallNum->setColor(Color3B(220, 40, 50));
            }
            else if (!isRed && !smallFromColored) {
                smallNum->setColor(Color3B(25, 35, 40));
            }

            const float kSmallNumToCardH = 0.18f;
            float targetH = this->getContentSize().height * kSmallNumToCardH;
            float scale = targetH / smallNum->getContentSize().height;
            smallNum->setScale(scale);

            smallNum->setAnchorPoint({ 0.f, 1.f });
            const float kPaddingLT = 10.f;
            smallNum->setPosition(kPaddingLT, this->getContentSize().height - kPaddingLT);
            smallNum->setLocalZOrder(3);
            this->addChild(smallNum, 3);
        }
        else {
            CCLOG("Warning: 缺少小号数字资源 %s，已跳过角标", smallTry.c_str());
        }
    }

    //  触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


bool CardView::onTouchBegan(Touch* touch, Event* event) {
    Vec2 p = touch->getLocation();
    Vec2 rectP = convertToNodeSpace(p);
    Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);

    return rect.containsPoint(rectP);
}

void CardView::onTouchEnded(Touch* touch, Event* event) {
    if (_onClick) {
        _onClick(this);
    }
}