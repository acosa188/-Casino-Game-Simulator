/*Monte Carlo simulation of different kind of games*/
/*                                                 */
/*Written by Arjun Cosare, University of Calgary   */
/*                                                 */
/*Usage:                                           */
/*                                                 */

#include <stdio.h>
#include <stdlib.h>

#define NUMTESTS 10000000 /* Number of games played */

/* Debugging output */
//#define DEBUG 1


/*This Random Number Generator is                  */
/*Written by Cary Williamson, University of Calgary*/
/***********************************************************************/
/*                 RANDOM NUMBER GENERATION STUFF                      */
/***********************************************************************/

/* Parameters for random number generation. */
#define MAX_INT 2147483647       /* Maximum positive integer 2^31 - 1 */

/* Generate a random floating point value uniformly distributed in [0,1] */

int deck[52];
int cardsDealt;
int isFull;
int infi;
double sweetHeartWinning;

/*Name: init()                                                        */
/*Role: Initialize or resets the deck of cards                        */
void init(){
    for(int i = 0; i < 52; i++){
        deck[i] = 0;
    }
    cardsDealt = 0;
    isFull = 0;
}
float Uniform01()
  {
    float randnum;

    /* get a random positive integer from random() */
    randnum = (float) 1.0 * random();

    /* divide by max int to get something in (0..1)  */
    randnum = (float) randnum / (1.0 * MAX_INT); 

    return( randnum );
  }

/* Generate a random integer uniformly distributed in [low,high] */
int EquiLikely(low, high)
     int low, high;
  {
    float randnum;
    int answer;

    randnum = Uniform01();
    answer = low + randnum*(high - low + 1);

//#ifdef DEBUG
//    printf("EquiLikely(%d,%d) uses %8.6f to return %d\n",
//	   low, high, randnum, answer);
//#endif

    return( answer );
  }

/* Name: Card Dealer                                       */
/* Role: Deal a card at random from a 52 card deck         */
/*       or deals a card at random from an infinite deck   */
int DrawCardFromDeckDealer()
  {
    int card, suit, rank;
    char rankchar;
    char suitstring[10];

    card = EquiLikely(0,51);   /* use routine from the textbook */

    if(infi == 0){
      while(deck[card] == 1){
	card = EquiLikely(0,51); 
      }
      /* place on the deck queue*/
      deck[card] = 1;
      cardsDealt++;
    
      if(cardsDealt == 52){
        isFull = 1;
        init();
      }
    }
#ifdef DEBUG
    /* determine suit using ordering for Bridge card game */ 
    suit = card / 13;
    if( suit == 0 )
      strcpy(suitstring, "Clubs");
    else if( suit == 1 )
      strcpy(suitstring, "Diamonds");
    else if( suit == 2 )
      strcpy(suitstring, "Hearts");
    else if( suit == 3 )
      strcpy(suitstring, "Spades");
    else strcpy(suitstring, "WTF???");

    /* determine card rank value as well */
    rank = card % 13;
    if( rank == 0 )
      rankchar = 'A';
    else if( rank == 10 )
      rankchar = 'J';
    else if( rank == 11 )
      rankchar = 'Q';
    else if( rank == 12 )
      rankchar = 'K';
    else rankchar = '1' + rank;

    if( rank == 9 )
      printf("Card from dealer is the 10 of %s\n", suitstring);
    else printf("Card from dealer is the %c of %s\n", rankchar, suitstring);
#endif

    return( card );
  }  

/* Name:  Boss game simulator                                 */
/* Rules: The player draws one card. If the card is an ace,   */
/*        then the player wins $10. Otherwise, the player     */
/*	  loses.                                              */

double Boss()
{
  /* Generate a random integer from 1 - 13     */
  /* Which stands for different types of cards */
  /* From Ace to Jack                          */

  int numGames, wins, losses;
  int card;
  
  /* Initialization */
  srandom(1234567);
  wins = 0;
  losses = 0;
  numGames = 0;
  while(numGames < NUMTESTS){

    init();

    card = DrawCardFromDeckDealer();

    if (card % 13 == 0){
      wins++;
    }
    else{
      losses++;
    }
    numGames++;
  }

  return (100.0*wins/numGames); 
}

/*Name:  Twins game Simulator                                   */
/*Rules: The player draws two cards. If the two cards are       */
/*       exactly identical (i.e., suit, colour, and rank), then */ 
/*       the player wins $50. Otherwise, the player loses.      */ 

double Twins(){
  int numGames, wins, losses;
  int firstHand, secondHand;


  /* Initialization */
  numGames = 0;
  wins = 0;
  losses = 0;
  firstHand = 0;
  secondHand = 0;


  srandom(1234567);

  while(numGames < NUMTESTS){

        init();

        firstHand = DrawCardFromDeckDealer();
        secondHand = DrawCardFromDeckDealer();

    if(firstHand == secondHand)
	wins++;
    else
      losses++;
    
    numGames++;
  }

  return (100.0*wins/numGames);
}

/*Name:  Biggie game Simulator                                   */
/*Rules: The player draws two cards. If the second card drawn    */
/*       has strictly higher rank than the first card drawn,     */
/*       then the player wins $2. Otherwise, the player loses.   */
double Biggie(){
  int numGames, wins, losses;
  int firstHand, secondHand;

  /* Initialization */
  numGames = 0;
  wins = 0;
  losses = 0;
  firstHand = 0;
  secondHand = 0;
  srandom(1234567);
  
  while(numGames < NUMTESTS){

        init();

        firstHand = DrawCardFromDeckDealer();
        secondHand = DrawCardFromDeckDealer();
    if(firstHand < secondHand){
      wins++;
    }
    else{
      losses++;
    }
    numGames++;
  }

  return (100.0*wins/numGames);
}

/*Name:  Sweethearts game Simulator                                */
/*Rules: The player draws three cards. If at least one card is     */
/*       Hearts, then the player wins $k, where k is the number    */
/*       of drawn cards that are Hearts. Otherwise, the player     */
/*       loses.                                                    */
double Sweethearts(){
  int numGames, wins, losses;
  int hands[3];
  int heart;
  int hearts;


  for(int i = 0; i < 3; i++){
    hands[i] = 0;
  }

  heart = 3;
  numGames = 0;
  wins = 0;
  losses = 0;
  srandom(1234567);
  sweetHeartWinning = 0;
  
  while(numGames < NUMTESTS){

    init();


	hearts = 0;
	for(int i = 0; i < 3; i++){
	  
	  hands[i] = DrawCardFromDeckDealer();
	  if(hands[i] / 13 == heart){
	    hearts++;
	    sweetHeartWinning++;
	  }
	}
	
	if(hearts > 0)
	  wins++;
	else{
	  losses++;
	}
	
	numGames++;
  }


	return (100.0*wins/numGames);
  
}

/*Name:  Runny Game Simulator                                      */
/*Rules: The player draws five cards. If there is a subset of      */
/*       these cards that makes a run of three or more consecutive */
/*       card values (e.g., 5-6-7 or 10-J-Q), then the player wins */
/*       $5. Otherwise, the player loses.                          */
double Runny(){
  int arr[5];                                 //number of cards on hand
  int numGames, wins, losses, series;
  int fHand, sHand, tHand, foHand, fiHand;
  int stop;                                     //boolean for early exit

  /* Initialization */

  srandom(1234567);
  numGames = 0;
  wins = 0;
  losses = 0;
  series = 0;
  stop = 0;
  

  /* Initialize array */
  for(int i = 0; i < 5; i++){
      arr[i] = 0;
  }


  
  while(numGames < NUMTESTS){
  init();
  
  //just need the ranks, don't care about the suits
   for(int i = 0; i < 5; i++){
      arr[i] = DrawCardFromDeckDealer()%13;

   }

  // Bubble Sort Algorithm 
	for(int x=0; x < 5; x++)
	{
		for(int y=0; y <(5-x-1); y++)
		{                        
			if(arr[y]>arr[y+1])
			{

				int temp = arr[y];
				arr[y] = arr[y+1];
				arr[y+1] = temp;
			}
		}
	}
    
  // Checks for consecutive
       
  for(int j = 0; j < 3 && stop == 0; j++){
    if(arr[j] == (arr[j+1]) - 1 && arr[j+1] == (arr[j+2]) - 1)
      series = 1;
    else
      series = 0;
    if(series == 1){
      wins++;
      stop = 1;
    }
  }
  
  if(stop == 0)
    losses++;
  
  stop = 0;
  series = 0;
  numGames++;
  }
 

  return (100.0*wins/numGames);
}

/*Name:  Couple's Game                                             */
/*Rules: The player draws ten cards.                               */
/*       If the player has K and Q on hand, then the player wins   */
/*       $5. Otherwise player losses.                              */ 
double Couple(){
  int numGames, wins, losses;
  int rank, suit;
  int hands[10];

  
  /* Initialization */
  srandom(1234567);
  numGames = 0;
  wins = 0;
  losses = 0;

  for(int i = 0; i < 5; i++){
    hands[i] = 0;
  }

  while(numGames < NUMTESTS){
    init();
    
    for(int i = 0; i < 10; i++){
      hands[i] = DrawCardFromDeckDealer();
  
    /* Setting K and Q at 1st and 2nd spots */
    if((hands[i] % 13) == 12){
	int temp = hands[0];
	hands[0] = hands[i];
	hands[i] = temp;
      } else if((hands[i] % 13) == 11){
	int temp = hands[1];
	hands[1] = hands[i];
	hands[i] = temp;
      }
    }

    /* Checks */
    if(((hands[0] % 13) == 12) && ((hands[1] % 13) == 11 )){
	wins++;
    }else 
      losses++;
    numGames++;
  }


  return (100.0*wins/numGames);
}

void table(){

  
  printf("==================================================================================================================================\n");
  printf("                 Single Deck                 Average Wins in Dollars          Infinite Deck               Average Wins in Dollars \n");
  printf("               Estimated WinRate             In a Single Deck               Estimated Winrate             In an Infinite Deck     \n");
  printf("                    in(%)                          ($)                            in(%)                          ($)              \n");
  printf("==================================================================================================================================\n");
  infi = 0;
  printf("Boss             %8.2f",Boss());
  printf("                          %8.4f",((Boss()/100)*NUMTESTS*10 - NUMTESTS)/(NUMTESTS) );
  infi = 1;
  printf("                  %8.2f",Boss());
  printf("                          %8.4f\n",((Boss()/100)*NUMTESTS*10 - NUMTESTS) /(NUMTESTS));
  infi = 0;
  printf("Twins            %8.2f",Twins());
  printf("                          %8.4f",(((Twins()/100)*NUMTESTS*50 - NUMTESTS) /NUMTESTS));
  infi = 1;
  printf("                  %8.2f",Twins());
  printf("                          %8.4f\n",((Twins()/100)*NUMTESTS*50 - NUMTESTS) /(NUMTESTS)); 
  infi = 0;
  printf("Biggie           %8.2f",Biggie());
  printf("                          %8.4f",((Biggie()/100)*NUMTESTS*2 - NUMTESTS) /NUMTESTS);  
  infi = 1;
  printf("                  %8.2f",Biggie());
  printf("                          %8.4f\n",((Biggie()/100)*NUMTESTS*2 - NUMTESTS) /(NUMTESTS));  
  infi = 0;
  printf("Sweethearts      %8.2f",Sweethearts());
  printf("                          %8.4f",(sweetHeartWinning - NUMTESTS)/ NUMTESTS);   
  infi = 1;
  sweetHeartWinning = 0;
  printf("                  %8.2f",Sweethearts());
  printf("                          %8.4f\n",(sweetHeartWinning - NUMTESTS) /NUMTESTS);  
  infi = 0;
  printf("Runny            %8.2f",Runny());
  printf("                          %8.4f",((Runny()/100)*NUMTESTS*5 - NUMTESTS) /(NUMTESTS));  
  infi = 1;
  printf("                  %8.2f",Runny());
  printf("                          %8.4f\n",((Runny()/100)*NUMTESTS*5 - NUMTESTS) /(NUMTESTS));
  infi = 0;
  printf("Couple           %8.2f",Couple());
  printf("                          %8.4f",((Couple()/100)*NUMTESTS*5 - NUMTESTS) /(NUMTESTS));
  infi = 1;
  printf("                  %8.2f",Couple());
  printf("                          %8.4f\n",((Couple()/100)*NUMTESTS*5 - NUMTESTS) /(NUMTESTS));
  
}
int main(){
  /*Types of Games                     */
  /*Boss           ----------------- 0 */
  /*Twins          ----------------- 1 */
  /*Biggie         ----------------- 2 */
  /*Sweethearts    ----------------- 3 */
  /*Runny          ----------------- 4 */
  /*Couple         ----------------- 5 */
 

  /* Intialization */
  infi = 0;
  init();


  table();                             //contains all data in a neat table form
}
