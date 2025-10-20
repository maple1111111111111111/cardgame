#ifndef CARD_H
#define CARD_H

#include <string>

enum class Suit { HEART, DIAMOND, CLUB, SPADE };
enum class Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

class Card {
public:
    Card(Suit suit, Rank rank);

    Suit getSuit() const;
    Rank getRank() const;
    int getRankValue() const; // 
    std::string getCardName() const; // 获取卡牌名称，用于加载纹理
    std::string getSuitName() const;
    std::string getNumberName() const;

    bool canMatch(const Card& other) const; // 判断是否可以匹配

private:
    Suit _suit;
    Rank _rank;
};

#endif // CARD_H