#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "Model/Card.h"
#include <functional>

class CardView : public cocos2d::Sprite {
public:
    // ֻ����ͨ���˾�̬�����������󣬱��봫��Card����
    static CardView* create(const Card& card);

    // ��ֹĬ�Ϲ��캯�����ⲿ����
    CardView() = delete;  // ��ʽɾ��Ĭ�Ϲ��캯��
    ~CardView() override = default;  // ����Ĭ����������

    const Card& getCard() const { return _card; }
    void setOnClickCallback(std::function<void(CardView*)> callback) { _onClick = callback; }


private:
    // ˽�й��캯��
    CardView(const Card& card);  // ���ι��캯������ʼ��_card

    Card _card;
    std::function<void(CardView*)> _onClick;

    bool init() override;  // ��дinit
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // CARD_VIEW_H