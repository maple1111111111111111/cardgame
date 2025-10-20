#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Model/GameModel.h"
#include "View/GameView.h"

class GameController {
public:
    GameController(GameModel* model, GameView* view);

    void startGame();

private:
    GameModel* _model;
    GameView* _view;

    // �������Ƶ��
    void onHandCardClicked(int index);

    // ���������Ƶ��
    void onTableCardClicked(int index);

    // ������˰�ť���
    void onUndoClicked();
};

#endif // GAME_CONTROLLER_H