#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "SceneHighScore.h"
#include "Player.h"

using namespace std;

void HighScore(void)
{
	ifstream inHighScore;
	ofstream outHighScore;
	string data;
	//Player *player = Player::getInstance();
	//player->points;

	int Score[3];
	int first = 0;
	int second=0;
	int third=0;
	int PlayerScore;

	inHighScore.open("HighScore.txt");

	cout << "Input Score : " << endl;
	cin >> PlayerScore;

	if (inHighScore.is_open())
	{
		for (int i = 0; i < 3; i++)
		{
			getline(inHighScore, data);
			Score[i] = stoi(data);
		}
		inHighScore.close();
		

		for (int i = 0; i < 3; i++)
		{
			cout << Score[i] << endl;
		}
	}
	else
	{
		cout << "Error opening HighScore.txt" << endl;
	}
	if (Score[0] > Score[1] && Score[0] > Score[2])
	{
		first = Score[0];
	}
	else if (Score[1] > Score[0] && Score[1] > Score[2])
	{
		first = Score[1];
	}
	else if (Score [2] > Score[0] && Score[2] > Score[1])
	{
		first = Score[2];
	}

	if (Score[0] < Score[1] && Score[0] < Score[2])
	{
		third = Score[0];
	}
	else if (Score[1] < Score[0] && Score[1] < Score[2])
	{
		third = Score[1];
	}
	else if (Score[2] < Score[0] && Score[2] < Score[1])
	{
		third = Score[2];
	}

	if (Score[0] < Score[1] && Score[0] > Score[2] || Score[0] > Score[1] && Score[0] < Score[2])
	{
		second = Score[0];
	}
	else if (Score[1] < Score[0] && Score[1] > Score[2] || Score[1] > Score[0] && Score[1] < Score[2])
	{
		second = Score[1];
	}
	else if (Score[2] < Score[0] && Score[2] > Score[1] || Score[2] > Score[0] && Score[2] < Score[1])
	{
		second = Score[2];
	}
	
	if (PlayerScore > first && PlayerScore > second && PlayerScore > third)
	{
		third = second;
		second = first;
		first = PlayerScore;
		cout << "You are the best on the leaderboard!" << endl;
	}
	else if (PlayerScore < first && PlayerScore > second && PlayerScore > third)
	{
		third = second;
		second = PlayerScore;
		cout << "You are second on the leaderboard!" << endl;
	}
	else if (PlayerScore < first && PlayerScore < second && PlayerScore > third)
	{
		third = PlayerScore;
		cout << "You are third on the leaderboard!" << endl;
	}
	else
	{
		cout << "You did not beat the highscore. Try again" << endl;
	}

	outHighScore.open("HighScore.txt");

	if (outHighScore.is_open())
	{
			outHighScore << to_string(first) << "\n" << to_string(second) << "\n" << to_string(third);
	}
	outHighScore.close();

	cout << first << endl;
	cout << second << endl;
	cout << third << endl;
}