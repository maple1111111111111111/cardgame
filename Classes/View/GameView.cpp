#include "GameView.h"

using namespace cocos2d;

GameView* GameView::create(GameModel* model) {
    GameView* pRet = new(std::nothrow) GameView();
    if (pRet && pRet->init(model)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool GameView::init(GameModel* model) {
    if (!Layer::init()) {
        return false;
    }

    _model = model;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    float tableHeight = visibleSize.height * 0.7f;  
    float handHeight = visibleSize.height * 0.3f;  



    _tableArea = Node::create();
    _tableArea->setAnchorPoint(Vec2(0, 0));  // ê����Ϊ���½�
    _tableArea->setPosition(origin.x, origin.y + handHeight);
    _tableArea->setContentSize(Size(visibleSize.width, tableHeight));  // ��ȷ����ߴ�
    addChild(_tableArea);
    // ���������� ê��(0,0)
    auto tableBg = LayerColor::create(
        Color4B(169, 121, 68, 200),  // ��ɫ
        visibleSize.width,           
        tableHeight                   
    );
    tableBg->setAnchorPoint(Vec2(0, 0));  // ê�����½�
    tableBg->setPosition(0, 0);           
    _tableArea->addChild(tableBg, -1);    // �����ڵײ�


    // ê��(0,0)�����½�����(0, 0)
    _handArea = Node::create();
    _handArea->setAnchorPoint(Vec2(0, 0));  // ê����Ϊ���½�
    _handArea->setPosition(origin.x, origin.y);  
    _handArea->setContentSize(Size(visibleSize.width, handHeight)); 
    addChild(_handArea);

    // ���������� ê��(0,0)
    auto handBg = LayerColor::create(
        Color4B(154, 32, 134, 200),  // ��ɫ
        visibleSize.width,            
        handHeight                   
    );
    handBg->setAnchorPoint(Vec2(0, 0));  // ê�����½�
    handBg->setPosition(0, 0);           
    _handArea->addChild(handBg, -1);     // �����ڵײ�


    //// ���˰�ť
    //_undoButton = MenuItemImage::create(
    //    "undo_normal.png",
    //    "undo_selected.png",
    //    [this](Ref* sender) {
    //        if (_onUndoClicked) {
    //            _onUndoClicked();
    //        }
    //    }
    //);
   // ���� ���˰�ť������ handBg ���֮�� ����
    _undoButton = MenuItemImage::create(
        "undo_normal.png",
        "undo_selected.png",
        [this](Ref*) { if (_onUndoClicked) _onUndoClicked(); }
    );

    // Ŀ�����ش�С�������Ｔ�ɣ�
    const float kUndoTargetPx = 64.0f;                 // 48/56/64 ����
    const Size tex = _undoButton->getContentSize();
    const float scale = kUndoTargetPx / std::max(tex.width, tex.height);
    _undoButton->setScale(scale);

    // �� Menu �ӵ���������������ϵ����������Ӧ
    auto menu = Menu::create(_undoButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setLocalZOrder(10000);
    _handArea->addChild(menu, 10000);

    // ���ϽǶ��루�������ص���
    const Size handSz = _handArea->getContentSize();
    const float kMargin = 12.0f;
    _undoButton->setAnchorPoint(Vec2(1.f, 1.f));
    _undoButton->setPosition(handSz.width - kMargin, handSz.height - kMargin);



    //auto menu = Menu::create(_undoButton, nullptr);
    // ���� ���˰�ť������ handBg ���֮�� ����
    //_undoButton = MenuItemImage::create(
    //    "undo_normal.png",
    //    "undo_selected.png",
    //    [this](Ref*) { if (_onUndoClicked) _onUndoClicked(); }
    //);

    //// 1) �á�Ŀ�����سߴ硱���ư�ť��С�������Ｔ�ɣ�
    //const float kUndoTargetPx = 56.0f;           // �� ����� 64/72����С�� 48
    //const auto  tex = _undoButton->getContentSize();
    //const float scale = kUndoTargetPx / std::max(tex.width, tex.height);
    //_undoButton->setScale(scale);

    //// 2) ê����λ�ã����ڡ����������Ҳ��в�������Ӧ�ֱ��ʣ�
    //_undoButton->setAnchorPoint(cocos2d::Vec2(1.f, 0.5f));
    //const float kMargin = 12.0f;                  // �� �ұ߾ࣨ���أ�
    //const auto  handSz = _handArea->getContentSize();

    //// �� Menu ���أ����� Menu ���� handArea �ڲ�����ϵ(0,0)
    //auto menu = Menu::create(_undoButton, nullptr);
    //menu->setPosition(cocos2d::Vec2::ZERO);
    //menu->setLocalZOrder(10000);
    //_handArea->addChild(menu, 10000);

    //// ���ð�ť�� handArea �ڵ��������꣨�Ҳ��в���
    //_undoButton->setPosition(handSz.width - kMargin, handSz.height * 0.5f);

    /*menu->setPosition(origin.x + visibleSize.width - 50, origin.y + handHeight - 50);
    addChild(menu, 10); */

    return true;
}



void GameView::updateView() {
    // ��¼��ǰ�����Ƶ�λ��
    _cardPositions.clear();
    for (int i = 0; i < _tableCardViews.size(); ++i) {
        CardView* card = _tableCardViews[i];
        if (card) {
            _cardPositions[getCardUniqueId(card->getCard())] = card->getPosition();
        }
    }
    // �ռ���ǰ���е��Ƽ���λ��
    for (int i = 0; i < _handCardViews.size(); ++i) {
        CardView* card = _handCardViews[i];
        if (card) {
            _cardPositions[getCardUniqueId(card->getCard())] = card->getPosition();
        }
    }

    // ��ȡ�µ�������
    const auto& newTableCards = _model->getTableCards();
    const auto& newHandCards = _model->getHandCards();

    // �����µ�����ͼ����ӵ�����
    std::vector<CardView*> newTableCardViews;
    std::vector<CardView*> newHandCardViews;
    //������model������views
    handleTableCardViews(newTableCardViews);
    handleHandCardViews(newHandCardViews);


    // �Ƴ�������Ҫ����
    auto removeUnusedCards = [](std::vector<CardView*>& cards) {
        for (auto card : cards) {
            if (card) {
                // �����������Ƴ�
                card->runAction(Sequence::create(
                    FadeOut::create(0.2f),
                    RemoveSelf::create(),
                    nullptr
                ));
            }
        }
        cards.clear();
    };

    removeUnusedCards(_tableCardViews);
    removeUnusedCards(_handCardViews);

    // ��������ͼ�б�
    _tableCardViews = newTableCardViews;
    _handCardViews = newHandCardViews;
}

void GameView::layoutTableCards() {
    int count = _tableCardViews.size(); if (count == 0) return;
    Size tableSize = _tableArea->getContentSize();
    float centerX = tableSize.width / 2; // ���������� X
    float bottomY = 250; 
    float splitOffset = 150; 
    float leftBottomX = centerX - splitOffset; 
    float rightBottomX = centerX + splitOffset; 
    float topLeftStartX = 200; 
    float topLeftStartY = tableSize.height - 200;
    float topRightStartX = tableSize.width - 200; 
    float topRightStartY = tableSize.height - 200; 
    int leftCount = (count + 1) / 2; 
    int rightCount = count / 2; 
    // ���
    for (int i = 0; i < leftCount; ++i) {
        CardView* card = _tableCardViews[i];
        if (!card) continue;
        float progress = (leftCount == 1) ? 0.5f : static_cast<float>(i) / (leftCount - 1);
        float x = topLeftStartX + (leftBottomX - topLeftStartX) * progress;
        float y = topLeftStartY - (topLeftStartY - bottomY) * progress;
        float rotation = 0 - 20 * progress;
        // ����λ�á���ת�Ͳ㼶
        card->setPosition(x, y);
        card->setRotation(rotation);
        card->setLocalZOrder(i);
    }
    // �Ҷ�
    for (int i = 0; i < rightCount; ++i) {
        int cardIndex = leftCount + i; // �Ҷѿ�Ƭ����
        CardView* card = _tableCardViews[cardIndex];
        if (!card) continue;
        float progress = (rightCount == 1) ? 0.5f : static_cast<float>(i) / (rightCount - 1);
        float x = topRightStartX - (topRightStartX - rightBottomX) * progress;
        float y = topRightStartY - (topRightStartY - bottomY) * progress;
        float rotation = 0 + 20 * progress;
        // ����λ�á���ת�Ͳ㼶
        card->setPosition(x, y);
        card->setRotation(rotation);
        card->setLocalZOrder(i + leftCount); 
    }
}
void GameView::layoutHandCards() {
    int count = _handCardViews.size();
    if (count == 0) return;

    // ��ȡ�������ߴ�
    Size handSize = _handArea->getContentSize();

    float cardWidth = 100;
    float spacing = 20;
    float totalCardsWidth = (count - 1) * spacing + count * cardWidth;

    // ˮƽ��������
    float startX = (handSize.width - totalCardsWidth) / 2;
    float centerY = handSize.height / 2;  // ��ֱ����

    for (int i = 0; i < count-1; ++i) {
        CardView* card = _handCardViews[i];
        float x = startX + i * (cardWidth + spacing);
        card->setPosition(x, centerY);
    }
    //����ƥ�������
    CardView* lastCard = _handCardViews[count - 1];
    float x = startX + (count - 1 + 2) * (cardWidth + spacing);
    lastCard->setPosition(x, centerY);
}

std::string GameView::getCardUniqueId(const Card& card) {
    return card.getSuitName() + "_" + card.getNumberName();
}

Vec2 GameView::getTargetPositionForTableCard(int index) {
    Size tableSize = _tableArea->getContentSize();
    float centerX = tableSize.width / 2;
    float bottomY = 250;
    float splitOffset = 150;
    float leftBottomX = centerX - splitOffset;
    float rightBottomX = centerX + splitOffset;
    float topLeftStartX = 200;
    float topLeftStartY = tableSize.height - 200;
    float topRightStartX = tableSize.width - 200;
    float topRightStartY = tableSize.height - 200;
    int count = _model->getTableCards().size();
    int leftCount = (count + 1) / 2;

    if (index < leftCount) {
        float progress = (leftCount == 1) ? 0.5f : static_cast<float>(index) / (leftCount - 1);
        float x = topLeftStartX + (leftBottomX - topLeftStartX) * progress;
        float y = topLeftStartY - (topLeftStartY - bottomY) * progress;
        return Vec2(x, y);
    }
    else {
        int rightIndex = index - leftCount;
        int rightCount = count / 2;
        float progress = (rightCount == 1) ? 0.5f : static_cast<float>(rightIndex) / (rightCount - 1);
        float x = topRightStartX - (topRightStartX - rightBottomX) * progress;
        float y = topRightStartY - (topRightStartY - bottomY) * progress;
        return Vec2(x, y);
    }
}

Vec2 GameView::getTargetPositionForHandCard(int index) {
    Size handSize = _handArea->getContentSize();
    int count = _model->getHandCards().size();

    float cardWidth = 100;
    float spacing = 20;
    float totalCardsWidth = (count - 1) * spacing + count * cardWidth;
    float startX = (handSize.width - totalCardsWidth) / 2;
    float centerY = handSize.height / 2;

    if (index == count - 1) {
        return Vec2(startX + (count - 1 + 2) * (cardWidth + spacing), centerY);
    }
    else {
        return Vec2(startX + index * (cardWidth + spacing), centerY);
    }
}

// ִ���Ƶ��ƶ�����
void GameView::animateCardMovement(CardView* card, const Vec2& targetPos, std::function<void()> callback) {
    card->stopAllActions();
    auto moveTo = MoveTo::create(0.3f, targetPos);
    auto sequence = Sequence::create(
        moveTo,
        CallFunc::create([callback]() {
            if (callback) callback();
            }),
        nullptr
                );
    card->runAction(sequence);
}

void GameView::handleTableCardViews(std::vector<CardView*>& newTableCardViews){
    const auto& newTableCards = _model->getTableCards();
    // ����������,�޸�_tablecardView��handview��,ֻʣ����Ҫ��������ֽ��
    for (int i = 0; i < newTableCards.size(); ++i) {
        const Card& card = newTableCards[i];
        std::string cardId = getCardUniqueId(card);
        // �����Ƿ����ֳɵ�����ͼ���Ը���
        CardView* cardView = nullptr;
        for (auto it = _tableCardViews.begin(); it != _tableCardViews.end(); ) {
            if (getCardUniqueId((*it)->getCard()) == cardId) {
                cardView = *it;
                it = _tableCardViews.erase(it);
                break;
            }
            else {
                ++it;
            }
        }
        // ���û���ҵ��������������Ƿ���
        if (!cardView) {
            for (auto it = _handCardViews.begin(); it != _handCardViews.end();++it ) {
                if (getCardUniqueId((*it)->getCard()) == cardId) {
                    cardView = *it;
                    it = _handCardViews.erase(it);
                    // �ֶ������ڵ㣬��ֹ���ͷ�
                    cardView->retain();
                    cardView->removeFromParent();
                    _tableArea->addChild(cardView);
                    cardView->release();
                    break;
                }     
            }
        }
        // �����Ȼû�У��򴴽��µ�����ͼ
        if (!cardView) {
            cardView = CardView::create(card);
            _tableArea->addChild(cardView);
            Vec2 targetPos = getTargetPositionForTableCard(i);
            cardView->setPosition(targetPos + Vec2(0, 200));
        }
        cardView->setOnClickCallback([this, i](CardView* view) {
            if (_onTableCardClicked) {
                _onTableCardClicked(i);
            }
            });
        // ��¼Ŀ��λ�ò�ִ�ж���
        Vec2 targetPos = getTargetPositionForTableCard(i);
        animateCardMovement(cardView, targetPos);
        newTableCardViews.push_back(cardView);
    }
}
void GameView::handleHandCardViews(std::vector<CardView*>& newHandCardViews) {
    const auto& newHandCards = _model->getHandCards();
    for (int i = 0; i < newHandCards.size(); ++i) {
        const Card& card = newHandCards[i];
        std::string cardId = getCardUniqueId(card);
        // �����Ƿ����ֳɵ�����ͼ���Ը���
        CardView* cardView = nullptr;
        for (auto it = _handCardViews.begin(); it != _handCardViews.end(); it++) {
            if (getCardUniqueId((*it)->getCard()) == cardId) {
                cardView = *it;
                it = _handCardViews.erase(it);
                break;
            }      
        }
        if (!cardView) {
            for (auto it = _tableCardViews.begin(); it != _tableCardViews.end(); it++) {
                if (getCardUniqueId((*it)->getCard()) == cardId) {
                    cardView = *it;
                    it = _tableCardViews.erase(it);
                    // ���ƴ��������ƶ���������

                    // �ֶ������ڵ㣬��ֹ���ͷ�
                    cardView->retain();
                    cardView->removeFromParent();
                    _handArea->addChild(cardView);
                    // �ͷ��ֶ�����������
                    cardView->release();
                    break;
                }      
            }
        }
        // �����Ȼû�У��򴴽��µ�����ͼ
        if (!cardView) {
            cardView = CardView::create(card);
            _handArea->addChild(cardView);
            // ���ó�ʼλ��
            Vec2 targetPos = getTargetPositionForHandCard(i);
            cardView->setPosition(targetPos + Vec2(0, -200));
        }
        // ���õ���ص�
        cardView->setOnClickCallback([this, i](CardView* view) {
            if (_onHandCardClicked) {
                _onHandCardClicked(i);
            }
            });
        // ��¼Ŀ��λ�ò�ִ�ж���
        Vec2 targetPos = getTargetPositionForHandCard(i);
        animateCardMovement(cardView, targetPos);
        newHandCardViews.push_back(cardView);
    }
}