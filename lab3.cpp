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

/**************** Part 3 ****************/

/* 
 * Gauss-Jordan Elimination, this is just a tool function, no need to comment
 */
vector<double> gauss(vector<vector<double>> equations)
{
    int row = equations.size();
    int column =  row + 1;

    vector<double> res(row, 0);
	for (int i = 0; i < row; i++){
		for (int j = i + 1; j < row; j++){
			double t = -equations.at(j).at(i)/equations.at(i).at(i);
		    vector<double> tmp(column, 0);

			for (int k = 0; k < column; k++)
				tmp.at(k) = equations.at(i).at(k) * t + equations.at(j).at(k);

			for (int k = 0; k < column; k++)
				equations.at(j).at(k) = tmp.at(k);
		}
	}

	for (int i = row - 1; i >= 0; i--){
		double t = equations.at(i).at(column - 1);
		for (int j = column - 2; j > i; j--)
			t -= (equations.at(i).at(j) * res.at(j));

		res.at(i) = t/equations.at(i).at(i);
	}

	return res;
}

/* 
 * n: count of nodes
 * hp: flood value
 * damage.at(i): hp that z costs when arriving node i
 * edges: there is an edge between node edges.at(i) and edges.at(i+1) where i%2 = 0
 */
double func3(int n,int hp,vector<int>& damage,vector<int>& edges) 
{
	int max = 0;// max damage
    for(int i = 0; i < damage.size(); ++i)
		max = damage.at(i) > max ? damage.at(i) : max;

	if(!max)// damages are all 0
		return 1;
	
	// constrtuct n*(hp+1) matrix as f(i,j)
	vector<vector<double>> f;
	for(int i = 0; i <= hp; ++i){
		vector<double> tmp;
		for(int j = 0; j < n; ++j)
			tmp.push_back(damage.at(j) > i ? 0 : -1);

		f.push_back(tmp);
	}
	f.at(0).at(0) = 1;

	// build map accoring to edges
	vector<vector<int>> map;
	for(int i = 0; i < n; ++i){
		vector<int> tmp;
		map.push_back(tmp);
	}
	for(int i = 0; i < edges.size(); i+=2){
		map.at(edges.at(i) - 1).push_back(edges.at(i + 1) - 1);
		map.at(edges.at(i + 1) - 1).push_back(edges.at(i) - 1);
	}
	
	// using Gauss-Jordan method to calcute equations of every row 
	for(int h = 0; h <= hp; ++h){
		vector<vector<double>> equations;
		for(int i = 0; i < n; ++i){// n equations 
			vector<double> equation(n + 1, 0);

			// init equations, 1 in augmented matrix[i][i], rest are 0
			equation.at(i) = 1;
			if(f.at(h).at(i) != -1){// known
				equation.at(n) = f.at(h).at(i);
			}

			else {
				// according to tran-state equation, fill augmented matrix[i][]
				for(int j = 0; j < map.at(i).size(); ++j){
					int sj = map.at(i).at(j);
					int k  = map.at(sj).size();
					double pj = 1/(double)k;
					int preHP = h - damage[i];

					if(sj != n-1 && preHP != hp){

						// constant term, add to augmented matrix[i][n]
						if(f.at(preHP).at(sj) != -1)
							equation.at(n) += f.at(preHP).at(sj)*pj;

						// variable item, fill augmented matrix[i][sj] with -pj which is the chance from node sj to node i
						else
							equation.at(sj) = -pj;
					}
				}
			}

			equations.push_back(equation);
		}

		// put tmp result back to f
		vector<double> tmpres = gauss(equations);
		for(int i = 0; i < tmpres.size(); ++i)
			f.at(h).at(i) = tmpres.at(i);

		for(int i = 0; i < equations.size(); ++i)
			equations.at(i).clear();
		tmpres.clear();
		equations.clear();
	}	

	double res = 0;
	for(int i = 0; i <= hp; ++i)
		res += f.at(i).at(n-1);

	// cout << "result: " << res << endl;
    return res;
}