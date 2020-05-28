#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

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

int main()
{
    int n;
    cin >> n;
    vector<vector<double>> equations;
    for(int i = 0; i < n; ++i){
        vector<double> equation;
        for(int j = 0; j <= n; ++j){
            double var;
            cin >> var;
            equation.push_back(var);
        }
        equations.push_back(equation);
    }

    vector<double> res = gauss(equations);
    for(int i = 0; i < n; ++i)
        cout << res.at(i) <<' ';

    return 0;
}