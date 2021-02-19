#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class Deck{
  vector<int> cards = {2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11};

public:
  void shuffle() {
      std::random_device random_device;
      std::mt19937 g(random_device());
      std::shuffle(cards.begin(), cards.end(), g);
    }

  int draw() {
      int card;
      shuffle();
      card = cards.back();
      cards.pop_back();
      return card;
    }

  void refresh() {
      cards = {2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11};
    }
};
class Hand{
  int count = 0;
  int aces = 0;
  int aceCounter = 0;
public:

  int getCount(){
    return count;
  }

  int getAces(){
    return aces;
  }

  void changeAceOverflow(){
    if (count > 21 && aceCounter > 0){
      count -= 10;
      aceCounter --;
    }
  }

  bool busted(){
    changeAceOverflow();
    return count > 21;
  };

  void hit(int card) {
    if (card == 11){
      aces++;
      aceCounter++;
    }
    count += card;
  };

  void refresh(){
    count = 0;
    aces = 0;
    aceCounter = 0;
  };
};

class Dealer{
  Deck deck;
  Hand dealerHand;
  Hand playerHand;
  bool playing;
public:

  Hand getDealerHand(){
    return dealerHand;
  }

  void setDeck(Deck d){
    deck = d;
  };

  void setPlayerHand(Hand p){
    playerHand = p;
  }

  void setDealerHand(Hand d){
    dealerHand = d;
  }

  void setPlaying(bool p){
    playing = p;
  }

  bool dealerWins(){
    return dealerHand.getCount() >= playerHand.getCount() && dealerHand.getCount() <=21;
  }

  bool isPlaying(){
    return playing;
  };

  void play(){
    if (dealerWins()) {
      playing = false;
    }
    else{
      dealerHand.hit(deck.draw());
      if (dealerHand.busted()) {
        playing = false;
      }
    }
  };
};

int main(int argc, char const *argv[]) {
  bool playerPlaying = true;
  int chips = 100;
  int bet = 1;
  int choice;
  Deck deck;
  Dealer dealer;
  Hand playerHand;
  Hand dealerHand;

  cout << "-------------------------------------------------------------" << endl;
  cout << "\n\t\t\tBlackJack!" << endl;

  while (chips > 0 && bet > 0){
    cout<<"\nYou have: " << chips << " chips.\nEnter bet or 0 to quit: ";
    cin >> bet;
    if (bet == 0) break;

    playerPlaying = true;
    deck.refresh();
    playerHand.refresh();
    dealerHand.refresh();


    playerHand.hit(deck.draw());
    playerHand.hit(deck.draw());
    playerHand.changeAceOverflow();
    dealerHand.hit(deck.draw());


    while (playerPlaying) {
      cout<<"\nYou have "<< playerHand.getCount()<<", "<< playerHand.getAces()<<" ace(s). Dealer has "<<dealerHand.getCount()<<".\nPress 0 to stay, 1 to hit: ";
      cin >> choice;
      if (choice == 1){
        playerHand.hit(deck.draw());
        if(playerHand.busted()) {
          playerPlaying = false;
          cout<<"\nYou have "<< playerHand.getCount()<<". You Lose." <<endl;
          chips -= bet;
        }
      }
      else{
        playerPlaying = false;

        dealer.setDealerHand(dealerHand);
        dealer.setPlayerHand(playerHand);
        dealer.setDeck(deck);
        dealer.setPlaying(true);

        while (dealer.isPlaying()){
          dealer.play();
          cout << "Dealer: " << dealer.getDealerHand().getCount() << ", " << dealer.getDealerHand().getAces() << " aces" << endl;
        }

        string statement = (dealer.dealerWins()) ? ". You Lose!":". You Win!";
        chips = (dealer.dealerWins()) ? chips-bet:chips+bet;
        cout<<"\nDealer has " <<dealer.getDealerHand().getCount()<< statement <<endl;
      }
    }
  }

  if (bet == 0) cout << "\nThanks for playing!" << endl;
  else if (chips < 1) cout<<"\nYou are out of chips, bum.\n";
  return 0;
}
