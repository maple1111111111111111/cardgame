#include "GameController.h"

GameController::GameController(GameModel* model, GameView* view)
    : _model(model), _view(view) {
    // 设置视图回调
    _view->setOnHandCardClicked(std::bind(&GameController::onHandCardClicked, this, std::placeholders::_1));
    _view->setOnTableCardClicked(std::bind(&GameController::onTableCardClicked, this, std::placeholders::_1));
    _view->setOnUndoClicked(std::bind(&GameController::onUndoClicked, this));
}

void GameController::startGame() {
    _model->initializeGame();
    _view->updateView();
}

void GameController::onHandCardClicked(int index) {
    if (_model->moveHandCardToTop(index)) {
        _view->updateView();
    }
}

void GameController::onTableCardClicked(int index) {
    if (_model->moveTableCardToHand(index)) {
        _view->updateView();
    }
}

void GameController::onUndoClicked() {
    if (_model->undoLastMove()) {
        _view->updateView();
    }
}