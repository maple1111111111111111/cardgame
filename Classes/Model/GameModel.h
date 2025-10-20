#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "Card.h"
#include <vector>
#include <stack>
#include <memory>
#include "UndoModel.h"
class GameModel {
public:
    GameModel();

    const std::vector<Card>& getTableCards() const;
    const std::vector<Card>& getHandCards() const;
    const Card* getTopHandCard() const;

    void initializeGame(); // 初始化游戏

    // 手牌区翻牌替换
    bool moveHandCardToTop(int index);

    // 桌面牌移动到手牌区顶部
    bool moveTableCardToHand(int index);

    // 回退操作
    bool undoLastMove();

private:
    std::vector<Card> _tableCards; // 桌面牌
    std::vector<Card> _handCards;  // 手牌
    //std::stack<MoveRecord> _moveHistory; // 操作历史
    UndoModel _undoModel;//回退
    std::vector<Card> generateRandomCards(int count);
};

#endif // GAME_MODEL_H