#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "Model/GameModel.h"
#include "CardView.h"
#include <vector>

class GameView : public cocos2d::Layer {
public:
    static GameView* create(GameModel* model);

    bool init(GameModel* model);

    // 更新视图以反映模型变化
    void updateView();

    // 设置回调函数
    void setOnHandCardClicked(std::function<void(int)> callback) { _onHandCardClicked = callback; }
    void setOnTableCardClicked(std::function<void(int)> callback) { _onTableCardClicked = callback; }
    void setOnUndoClicked(std::function<void()> callback) { _onUndoClicked = callback; }

private:
    GameModel* _model;
    cocos2d::Node* _tableArea;    // 桌面牌区域
    cocos2d::Node* _handArea;     // 手牌区域
    cocos2d::MenuItemImage* _undoButton; // 回退按钮

    std::vector<CardView*> _tableCardViews;
    std::vector<CardView*> _handCardViews;
    

    // 记录牌的位置信息用于动画
    std::unordered_map<std::string, cocos2d::Vec2> _cardPositions;
    // 获取牌的唯一标识
    std::string getCardUniqueId(const Card& card);
    // 获取牌的目标位置
    cocos2d::Vec2 getTargetPositionForTableCard(int index);
    cocos2d::Vec2 getTargetPositionForHandCard(int index);
    // 执行牌的移动动画
    void animateCardMovement(CardView* card, const cocos2d::Vec2& targetPos, std::function<void()> callback = nullptr);

    std::function<void(int)> _onHandCardClicked;
    std::function<void(int)> _onTableCardClicked;
    std::function<void()> _onUndoClicked;

    // 布局卡牌
    void layoutTableCards();
    void layoutHandCards();
    void GameView::handleTableCardViews(std::vector<CardView*>& newTableCardViews);
    void GameView::handleHandCardViews(std::vector<CardView*>& newHandCardViews);
};

#endif // GAME_VIEW_H