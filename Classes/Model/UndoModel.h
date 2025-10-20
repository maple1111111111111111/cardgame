// UndoModel.h
#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "Card.h"
#include <stack>

// 操作记录结构体
struct MoveRecord {
    enum class Type { HAND_TO_TOP, TABLE_TO_HAND } type;
    int cardIndex; // 操作的卡片索引
    Card previousTopCard; // 操作前的顶部卡片
  
    MoveRecord()
        : type(Type::HAND_TO_TOP)  // 初始化枚举类型
        , cardIndex(-1)            // 初始化索引为无效值
        , previousTopCard(Suit::HEART, Rank::ACE)  // 初始化Card
    {}
    MoveRecord(Type t, int idx, const Card& prevCard)
        : type(t)
        , cardIndex(idx)
        , previousTopCard(prevCard)
    {}
};

class UndoModel {
public:
    // 添加一条操作记录
    void pushRecord(const MoveRecord& record);

    // 获取并移除最后一条记录
    MoveRecord popRecord();
    bool popRecord(MoveRecord& outRecord);

    // 清空所有记录
    void clearRecords();

    // 判断是否有可回退的记录
    bool hasRecords() const;

private:
    std::stack<MoveRecord> _moveHistory; // 操作历史栈
};

#endif // UNDO_MODEL_H