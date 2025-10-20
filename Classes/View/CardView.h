#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "Model/Card.h"
#include <functional>

class CardView : public cocos2d::Sprite {
public:
    // 只允许通过此静态方法创建对象，必须传入Card参数
    static CardView* create(const Card& card);

    // 禁止默认构造函数的外部调用
    CardView() = delete;  // 显式删除默认构造函数
    ~CardView() override = default;  // 保持默认析构函数

    const Card& getCard() const { return _card; }
    void setOnClickCallback(std::function<void(CardView*)> callback) { _onClick = callback; }


private:
    // 私有构造函数
    CardView(const Card& card);  // 带参构造函数，初始化_card

    Card _card;
    std::function<void(CardView*)> _onClick;

    bool init() override;  // 重写init
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // CARD_VIEW_H