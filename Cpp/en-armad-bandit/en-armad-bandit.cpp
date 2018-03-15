#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <fstream>

using namespace std;

bool gameRunning = true;
bool error;
int wins;
string user;
string player_password;

void generateBandit(char array[3][3]);
void printBandit(char array[3][3]);
int countWins(int type, char array[3][3]);
int generateBalance(int &player_balance);
int generateBet(int &player_bet, int &player_balance);
void printResults(int &rows, int &columns, int &diagonals);
int payOff(int &wins, int &player_bet, int &player_balance, int &player_result);
void continuePlay(int &player_balance, int &player_result);

void trackUser(int &player_balance, string &player_password, int &player_result);
void generateUser(int &player_balance, int &player_result);

int main ()
{
	//Variables
	char banditArray[3][3];
	int player_balance;
	int player_bet;
	int player_result;
	
	int rows;
	int columns;
	int diagonals;
	
	
	srand(time(NULL));
	cout << "Welcome to the One-Armed Bandit!" << endl;
	generateUser(player_balance, player_result);
	while(gameRunning){
		generateBalance(player_balance);
		generateBet(player_bet, player_balance);
	
		generateBandit(banditArray);
		printBandit(banditArray);
		
		rows = countWins(0, banditArray);
		columns = countWins(1, banditArray);
		diagonals = countWins(2, banditArray);
		wins = rows + columns + diagonals;
		printResults(rows, columns, diagonals);
		payOff(wins, player_bet, player_balance, player_result);
		
		continuePlay(player_balance, player_result);
	}
	trackUser(player_balance, player_password, player_result);
	cout << "Thanks for playing the game!" << endl;
	system("pause");
}

// ** Generate User Information ** //
void generateUser(int &player_balance, int &player_result){
	int previous_player_bank;
	int previous_result;
	
	string previous_player_password;
	string userFile;
	string player_input;
	
	bool activeUser = false;
	
	while(!activeUser){
		cout << "What is your name? " << endl;
		cout << "Name: ";
		cin >> user;
		
		
		userFile = user;
		userFile += ".txt";
		
		
		ifstream getFile(userFile.c_str()); // Checks for previous username and tries to open the file,
		if(getFile.is_open()){				// if the file does not exists, prompts the user to associate a password.
			do{
				error = true;
				for(int i = 1; i <= 3; i++){
					cout << "Please enter the password associated with the account." << endl;
					cout << "Password: ";
					cin >> player_input;
					
					getFile >> previous_player_bank;
					getFile >> previous_player_password;
					getFile >> previous_result;
					
					player_balance = previous_player_bank;
					player_password = previous_player_password;	
					
					
					if(player_input == player_password){
						system("cls");
						cout << "Hello " << user << " you currently have " << player_balance << " on your account!" << endl;
						cout << "Your result for your last session was " << previous_result << endl;
						error = false;
						activeUser = true;
						break;
					}
					else
					{
						cout << "Sorry these passwords do not match" << endl;
					}
					
					if( i >= 3){
						system("cls");
						error = false;
						cout << "You have entered the wrong password 3 times, out of security reasons you will need to enter your name again." << endl;
					}
				}
			}
			while(error);
			
		}
		else
		{
			cout << "There seems to be no previous record of you playing here!" << endl;
			cout << "Please enter a password to your account." << endl;
			cout << "Password: ";
			cin >> player_password;
			activeUser = true;
		}
		getFile.close();
	}

	
}

// ** Track User Information ** //
void trackUser(int &player_balance, string &player_password, int &player_result)
{
	string userFile;
	userFile = user;
	userFile += ".txt";
	ofstream storefile;
	storefile.open (userFile.c_str()); //creates a file with the name associated to the user variable and stores 
	storefile << player_balance << " "; //balance, password and the players result for the session.
	storefile << player_password << " ";
	storefile << player_result;
	
	storefile.close();
}


int generateBalance(int &player_balance){
	if(player_balance <= 0){
		do{
			error = true;
			
			cout << "How much money would you like to deposit into your account? (100,300 or 500)" << endl;
			cout << "Deposit: ";
			cin >> player_balance;
			
			while (cin.fail()) //Checks so the user inputs valid input for an int variable
	   		{
	        	cin.clear();
	        	cin.ignore(INT_MAX, '\n');
	        	cout << "You may only enter numbers" << endl;
	        	cout << "Deposit: ";
	        	cin >> player_balance;
	 	    }	
	 	    
	 	    if(player_balance != 100 && player_balance != 300 && player_balance != 500){
	 	    	//Value is invalid
	 	    	cout << "That is an invalid amount, you may only insert 100,300 or 500" << endl;
			 }
			 else
			 {
			 	//Value is either 100, 300 or 500
			 	cout << "You decided to add " << player_balance << " to your account!" << endl;
			 	error = false;
			 }
		}
		while(error);
	}
	return player_balance;
}

int generateBet(int &player_bet, int &player_balance){
	do{
			error = true;
			
			cout << "How much would you like to bet on this bandit game?" << endl;
			cout << "Bet: ";
			cin >> player_bet;
			
			while (cin.fail()) //Checks so the user inputs valid input for an int variable
	   		{
	        	cin.clear();
	        	cin.ignore(INT_MAX, '\n');
	        	cout << "You may only enter numbers" << endl;
	        	cout << "Bet: ";
	        	cin >> player_balance;
	 	    }	
	 	    
	 	    if(player_bet > player_balance){
	 	    	cout << "You cannot bet more than a your balance, your balance is currently "
				 << player_balance << "!"  << endl;
			 }
			 else if(player_bet <= 0)
			 {
			 	cout << "You cannot bet zero or negative values!" << endl;
			 }
			 else
			 {
			 	cout << "You decided to bet " << player_bet << " on this game" << endl;
			 	player_balance -= player_bet;
			 	cout << "Your balance is currently " << player_balance << endl;
			 	error = false;
			 }
		}
		while(error);
		
		return player_bet;
}



void generateBandit(char array[3][3])
{
	//loops through and randomizing a symbol in each spot of the array.
	for(int i = 0; i < 3; i++)
	{
		
		for(int j = 0; j < 3; j++)
		{
			switch(rand() % 3)
			{
				case 0:
					array[i][j] = 'X';
					break;
				case 1: 
					array[i][j] = 'Y';
					break;
				case 2:
					array[i][j] = 'Z';
					break;
			}
		}
		
	}
}

void printBandit(char array[3][3]){
	for(int i = 0; i < 3; i++)
	{	
		for(int j = 0; j < 3; j++)
		{
			cout << "[" << array[i][j] << "]";
		}
		
		cout << endl;
	}
}

int countWins(int type, char array[3][3])
{
	int wins = 0;
	switch(type)
	{
		case 0: //row
		
			/*  -->
				i x x  
				i x x
				i x x
			*/
			for (int i = 0; i < 3; i++){
				if(array[i][0] == array[i][1] && array[i][1] == array[i][2])
				{
					wins ++;
				}
			}
			break;
		
		case 1: //Column
		
		/*
			i i i
			x x x
			x x x
		*/
			for (int i = 0; i < 3; i++){
				if(array[0][i] == array[1][i] && array[1][i] == array[2][i])
				{
					wins ++;
				}
			}
			break;
			
		case 2: //Diagonals
		
			/*
				i		(0,0)
				  i		(1,1)
					i	(2,2)
			*/
			if(array[0][0] == array[1][1] && array[1][1] == array[2][2]){
				wins ++;
			}
			
			/*
					i	(0,2)
				  i		(1,1)
				i		(2,0)
			*/
			if(array[0][2] == array[1][1] && array[1][1] == array[2][0]){
				wins ++;
			}
			
			break;
	}
	
	return wins;
}

void printResults(int &rows, int &columns, int &diagonals){
	cout << "These are the results for this game!" << endl;
	cout << "* * * * *" << endl;
	cout << " Rows: " << rows << endl;
	cout << " Columns: " << columns << endl;
	cout << " Diagonals: " << diagonals << endl;
	cout << endl;
	cout << " Total wins: " << wins << endl;
	cout << "* * * * *" << endl;
}

int payOff(int &wins, int &player_bet, int &player_balance, int &player_result){
	int maxWins = 8;
	int payOff = 0;

	if(wins > 0)
	{
		if( wins < 5)
		{
			payOff = player_bet * (1 + wins );
		}
		else if(wins > 5 < maxWins)
		{
			payOff = player_bet * (2 + wins);
		
		}else
		{
			payOff = player_bet * 10;
		}
		
		cout << "You won a total of "<< payOff << endl;
		//Transfer funds to player account
		player_balance += payOff;
		player_result += payOff;
		cout << "Your balance is currently "<< player_balance << endl;
	}
	else
	{
		cout << "No rows, columns, or diagonal wins." << endl;
		cout << "You lost " << player_bet << ", your current balance is " << player_balance << endl;
		player_result -= player_bet;
	}
	return player_balance;
}

void continuePlay(int &player_balance, int &player_result){
	do{
		error = true;
		
		string player_answer;
		cout << "Do you wish to keep playing?(yes/no) ";
		cin >> player_answer;
		
		for(int i = 0; i < player_answer.length(); i++){
			player_answer[i] = tolower(player_answer[i]);
		}
		
		if(player_answer == "yes" || player_answer == "y")
		{
			error = false;
			if(player_balance <= 0){
				system("cls");
				cout << "You account seems to be empty" << endl;
			}
			else
			{
			system("cls");
			cout << "Your balance is currently " << player_balance << endl;
			}
		}
		else if(player_answer == "no" || player_answer == "n")
		{
			cout << endl;
			cout << "Your total balance is " << player_balance << endl;
			cout << "Your result for this session is " << player_result << endl;
			error = false;
			gameRunning = false;
		}
		else
		{
			cout << "Please enter valid input" << endl;
		}
	}
	while(error);
}
