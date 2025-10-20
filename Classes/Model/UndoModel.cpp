// UndoModel.cpp
#include "UndoModel.h"

void UndoModel::pushRecord(const MoveRecord& record) {
    _moveHistory.push(record);
}

bool UndoModel::popRecord(MoveRecord& outRecord) {
    if (hasRecords()) { // �м�¼ʱ����ȡ������
        outRecord = _moveHistory.top();
        _moveHistory.pop();
        return true; // �ɹ�
    }
    // �޼�¼ʱ������false
    return false;
}

void UndoModel::clearRecords() {
    while (!_moveHistory.empty()) {
        _moveHistory.pop();
    }
}

bool UndoModel::hasRecords() const {
    return !_moveHistory.empty();
}