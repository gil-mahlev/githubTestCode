#include <iostream>     // std::cout
#include <functional>   // std::minus
#include <numeric>      // std::accumulate
#include <vector>
#include <algorithm>
#include <string> 
#include <cstddef>

using namespace std;



void create_double_list(void )
{
	
	vector<vector<string>> matrixVector = {
                                {"1", "2", "3", "4", "5" },
                                {"6", "7", "8", "9", "10" },
                                {"5", "6", "8", "1", "12" },
                                {"1", "7", "2", "4", "18" },
                            };
							
	vector<string>newVector = {
		"qqq","eee","ttt"
	};
	matrixVector.push_back(newVector);
	
	
    for(auto row_obj : matrixVector)
    {
        for (auto elem: row_obj)
        {
            cout<<elem<<", ";
        }
        cout<<endl;
    }
	
	while(matrixVector.empty() == 0)
	{
		vector<string> popvector = matrixVector.back();
		matrixVector.pop_back();
		for(auto elem: popvector)
		{
			cout << elem << ", ";
		}
		cout << endl;
	}
}

template <typename M,typename T>
class pointPosition
{
private:
	M x;
	T y;
public:
	pointPosition(M o,T v){x=o;y=v;}
	M getPositionX(){return x;}
	T getPositionY(){return y;}
};

void create_class_vector(void )
{
	int matrix_size=0;
	vector<vector<pointPosition<char,int>>> matrixPosition = {
							{{'A',1},{'A',2},{'A',3},{'A',4},{'A',5} },
							{{'B',1},{'B',2},{'B',3},{'B',4},{'B',5} },
							{{'C',1},{'C',2},{'C',3},{'C',4},{'C',5} },
							{{'D',1},{'D',2},{'D',3},{'D',4},{'D',5} },
	};
	
	for (auto raw : matrixPosition)
	{
		matrix_size += raw.size();
		for (auto elem : raw)
		{
			cout <<" "<< elem.getPositionX()<<":"<<elem.getPositionY()<<",";
		}
		cout << endl;
	}
	cout << "number of elements in the matrix vector:"<<matrix_size<<endl;
	
	
	for (auto it = matrixPosition.rbegin(); it != matrixPosition.rend(); ++it)
	{
		for (auto elem = it->rbegin(); elem != it->rend(); ++elem)
		{
			cout <<" "<< elem->getPositionX()<<":"<<elem->getPositionY()<<",";
		}
		cout << endl;
	}
	
}



