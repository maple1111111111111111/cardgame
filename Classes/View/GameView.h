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

    // ������ͼ�Է�ӳģ�ͱ仯
    void updateView();

    // ���ûص�����
    void setOnHandCardClicked(std::function<void(int)> callback) { _onHandCardClicked = callback; }
    void setOnTableCardClicked(std::function<void(int)> callback) { _onTableCardClicked = callback; }
    void setOnUndoClicked(std::function<void()> callback) { _onUndoClicked = callback; }

private:
    GameModel* _model;
    cocos2d::Node* _tableArea;    // ����������
    cocos2d::Node* _handArea;     // ��������
    cocos2d::MenuItemImage* _undoButton; // ���˰�ť

    std::vector<CardView*> _tableCardViews;
    std::vector<CardView*> _handCardViews;
    

    // ��¼�Ƶ�λ����Ϣ���ڶ���
    std::unordered_map<std::string, cocos2d::Vec2> _cardPositions;
    // ��ȡ�Ƶ�Ψһ��ʶ
    std::string getCardUniqueId(const Card& card);
    // ��ȡ�Ƶ�Ŀ��λ��
    cocos2d::Vec2 getTargetPositionForTableCard(int index);
    cocos2d::Vec2 getTargetPositionForHandCard(int index);
    // ִ���Ƶ��ƶ�����
    void animateCardMovement(CardView* card, const cocos2d::Vec2& targetPos, std::function<void()> callback = nullptr);

    std::function<void(int)> _onHandCardClicked;
    std::function<void(int)> _onTableCardClicked;
    std::function<void()> _onUndoClicked;

    // ���ֿ���
    void layoutTableCards();
    void layoutHandCards();
    void GameView::handleTableCardViews(std::vector<CardView*>& newTableCardViews);
    void GameView::handleHandCardViews(std::vector<CardView*>& newHandCardViews);
};

#endif // GAME_VIEW_H