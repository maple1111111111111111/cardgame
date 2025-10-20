// UndoModel.cpp
#include "UndoModel.h"

void UndoModel::pushRecord(const MoveRecord& record) {
    _moveHistory.push(record);
}

bool UndoModel::popRecord(MoveRecord& outRecord) {
    if (hasRecords()) { // 有记录时，获取并弹出
        outRecord = _moveHistory.top();
        _moveHistory.pop();
        return true; // 成功
    }
    // 无记录时，返回false
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