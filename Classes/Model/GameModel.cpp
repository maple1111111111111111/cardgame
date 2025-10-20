#include "GameModel.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

GameModel::GameModel() {
    srand(time(nullptr));
}

const std::vector<Card>& GameModel::getTableCards() const {
    return _tableCards;
}

const std::vector<Card>& GameModel::getHandCards() const {
    return _handCards;
}

const Card* GameModel::getTopHandCard() const {
    return _handCards.empty() ? nullptr : &_handCards.back();
}

void GameModel::initializeGame() {
    // 清空现有数据
    _tableCards.clear();
    _handCards.clear();
   // while (!_moveHistory.empty()) _moveHistory.pop();
   _undoModel.clearRecords();

   auto randomCards = generateRandomCards(10);
   _tableCards.assign(randomCards.begin(), randomCards.begin() + 6);
   _handCards.assign(randomCards.begin() + 6, randomCards.end());

}

bool GameModel::moveHandCardToTop(int index) {
    if (index < 0 || index >= _handCards.size() || index == _handCards.size() - 1) {
        return false; // 无效索引或已是顶部牌
    }

    // 记录操作
    MoveRecord record = {
    MoveRecord::Type::HAND_TO_TOP,
    index,
    _handCards.back()  // 用现有卡牌初始化previousTopCard
    };

    // 将选中的手牌移到顶部
    Card selectedCard = _handCards[index];
    _handCards.pop_back();
    _handCards.erase(_handCards.begin() + index);
    _handCards.push_back(selectedCard);

   // _moveHistory.push(record);
    _undoModel.pushRecord(record);
    return true;
}

bool GameModel::moveTableCardToHand(int index) {
    if (index < 0 || index >= _tableCards.size()) {
        return false; // 无效索引
    }

    const Card& tableCard = _tableCards[index];
    const Card* topCard = getTopHandCard();

    // 检查是否可以匹配
    if (topCard && !tableCard.canMatch(*topCard)) {
        return false;
    }

    // 记录操作
    MoveRecord record = {
    MoveRecord::Type::TABLE_TO_HAND,
    index,
    (topCard ? *topCard : Card(Suit::HEART, Rank::ACE))  // 确保有效
    };

    // 将桌面牌移到手牌顶部
    Card selectedCard = _tableCards[index];
    _tableCards.erase(_tableCards.begin() + index);
    _handCards.pop_back();
    _handCards.push_back(selectedCard);
    _undoModel.pushRecord(record);
    //_moveHistory.push(record);
    return true;
}

bool GameModel::undoLastMove() {
    if (!_undoModel.hasRecords()) {
        return false;
    }
    MoveRecord record;
    _undoModel.popRecord(record);

    switch (record.type) {
    case MoveRecord::Type::HAND_TO_TOP: {
        // 将顶部牌移回原来的位置
        Card topCard = _handCards.back();
        _handCards.pop_back();
        _handCards.push_back(record.previousTopCard);
        _handCards.insert(_handCards.begin() + record.cardIndex, topCard);
        break;
    }
    case MoveRecord::Type::TABLE_TO_HAND: {
        // 将手牌顶部牌移回桌面
        Card topCard = _handCards.back();
        _handCards.pop_back();
        _tableCards.insert(_tableCards.begin() + record.cardIndex, topCard);

        // 如果之前有顶部牌，恢复它
        if (!_handCards.empty() || record.previousTopCard.getRankValue() > 0) {
            _handCards.push_back(record.previousTopCard);
        }
        break;
    }
    }

    return true;
}


std::vector<Card> GameModel::generateRandomCards(int count) {
    std::vector<Card> cards;
    if (count <= 0) return cards;

    std::vector<Suit> availableSuits = {
        Suit::DIAMOND,  // 方块
        Suit::SPADE,    // 黑桃
        Suit::HEART,    // 红桃
        Suit::CLUB      // 梅花
    };

    std::vector<Rank> availableRanks = {
        Rank::THREE,    // 3
        Rank::FOUR,     // 4
        Rank::FIVE,     // 5
        Rank::SIX,      // 6
        Rank::SEVEN,    // 7
        Rank::EIGHT,    // 8
        Rank::NINE,     // 9
        Rank::TEN,      // 10
        Rank::JACK,     // J
        Rank::QUEEN,    // Q
        Rank::KING,     // K
        Rank::ACE       // A
    };

    const int maxUniqueCards = availableSuits.size() * availableRanks.size();
    if (count > maxUniqueCards) {
        count = maxUniqueCards;
    }

    // 创建所有可能的牌组合
    std::vector<Card> allPossibleCards;
    for (const auto& suit : availableSuits) {
        for (const auto& rank : availableRanks) {
            allPossibleCards.emplace_back(suit, rank);
        }
    }

    // 随机洗牌
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // 初始化随机种子
    std::random_shuffle(allPossibleCards.begin(), allPossibleCards.end());

    // 取前count张牌
    cards.insert(cards.end(), allPossibleCards.begin(), allPossibleCards.begin() + count);

    return cards;
}
