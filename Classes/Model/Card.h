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
    std::string getCardName() const; // ��ȡ�������ƣ����ڼ�������
    std::string getSuitName() const;
    std::string getNumberName() const;

    bool canMatch(const Card& other) const; // �ж��Ƿ����ƥ��

private:
    Suit _suit;
    Rank _rank;
};

#endif // CARD_H