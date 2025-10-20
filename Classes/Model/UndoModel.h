// UndoModel.h
#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "Card.h"
#include <stack>

// ������¼�ṹ��
struct MoveRecord {
    enum class Type { HAND_TO_TOP, TABLE_TO_HAND } type;
    int cardIndex; // �����Ŀ�Ƭ����
    Card previousTopCard; // ����ǰ�Ķ�����Ƭ
  
    MoveRecord()
        : type(Type::HAND_TO_TOP)  // ��ʼ��ö������
        , cardIndex(-1)            // ��ʼ������Ϊ��Чֵ
        , previousTopCard(Suit::HEART, Rank::ACE)  // ��ʼ��Card
    {}
    MoveRecord(Type t, int idx, const Card& prevCard)
        : type(t)
        , cardIndex(idx)
        , previousTopCard(prevCard)
    {}
};

class UndoModel {
public:
    // ���һ��������¼
    void pushRecord(const MoveRecord& record);

    // ��ȡ���Ƴ����һ����¼
    MoveRecord popRecord();
    bool popRecord(MoveRecord& outRecord);

    // ������м�¼
    void clearRecords();

    // �ж��Ƿ��пɻ��˵ļ�¼
    bool hasRecords() const;

private:
    std::stack<MoveRecord> _moveHistory; // ������ʷջ
};

#endif // UNDO_MODEL_H