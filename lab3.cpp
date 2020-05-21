#include "common.h"

using namespace std;

//You should only code here.Don't edit any other files in this 
/*
 * amount: total value
 * coins.at(i): ith coin's face value
 * design idea in report docs
 */
int func1(int amount, vector<int>& coins)
{	
	if(!amount)
		return 1;// every kind of coin costs 0

	if(coins.empty())
		return 0;// amount > 0, but no coin

	// int sum = 1, coin = coins.at(0);
	vector<int> line0(amount + 1, 0), line1(amount + 1, 0);
	line0.at(0) = 1; // dp[0][0] = 1


	for(int i = 0; i < coins.size(); ++i){
		int coin = coins.at(i);
		for(int j = 0; j < amount + 1; ++j){
			for(int k = 0; k <= j/coin; ++k){
				line1.at(j) += line0.at(j - k*coin);
			}
		}

		for(int l = 0; l < amount + 1; ++l){
			line0.at(l) = line1.at(l);
			line1.at(l) = 0;
		}
	}
	
	return line0.at(amount);
}

/*
 * meet.at(i).at(j): i can meet j
 * conquer.at(i).at(j): i can defeat j
 */
int func2(int amount, vector< vector<int> >& conquer)
{
	// init meet matrix
	vector<vector<int>> meet;
	for(int i = 0; i < amount; ++i){
		vector<int> tmp(amount, 0);
		tmp.at((i + 1)%amount) = 1;
		meet.push_back(tmp);
	}

	// update meet matrix
	for(int i = 1; i < amount; ++i){
		for(int beg = 0; beg < amount; beg++){
			int end = (beg + i + 1)%amount;
			
			// accessable, next loop
			if(meet.at(beg).at(end))
				continue;
				
			// if exists j that beg can meet j and j can meet end, and beg or end 
			// can defeat j so that beg can meet end
			for(int j = (beg + 1)%amount; j != end; j++, j %= amount){
				if(meet.at(beg).at(j) && meet.at(j).at(end) && 
				(conquer.at(beg).at(j) || conquer.at(end).at(j))) {
					meet.at(beg).at(end) = 1;
					break;
				}
			}
		}
	}

	// meet.at(i).at(i) = 1 means i can win
	int count = 0;
	for(int i = 0; i < amount; ++i){
		count += meet.at(i).at(i);
		meet.at(i).clear();
	}

	meet.clear();
	return count;
}


double func3(int n,int hp,vector<int>& damage,vector<int>& edges) 
{
    
    return 1.0;
}