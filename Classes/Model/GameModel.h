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

    void initializeGame(); // ��ʼ����Ϸ

    // �����������滻
    bool moveHandCardToTop(int index);

    // �������ƶ�������������
    bool moveTableCardToHand(int index);

    // ���˲���
    bool undoLastMove();

private:
    std::vector<Card> _tableCards; // ������
    std::vector<Card> _handCards;  // ����
    //std::stack<MoveRecord> _moveHistory; // ������ʷ
    UndoModel _undoModel;//����
    std::vector<Card> generateRandomCards(int count);
};

#endif // GAME_MODEL_H