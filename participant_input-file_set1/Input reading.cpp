#include <bits/stdc++.h>

using namespace std;

struct row_t {
	string curr;
	int hour, minute, second;
	long long time;
	double bid, ask;
};

vector<row_t> bgg_data;

int main() {
	row_t row;
	
	ifstream bgg ( "bbg.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	ifstream ebs ( "ebs.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	ifstream reu ( "reu.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string curr;
	int i = 0;
	int hour, minute, second;
	double bid, ask;
	while ( bgg.good() ){		
		bgg >> row.hour; bgg.get();
		bgg >> row.minute; bgg.get();
		bgg >> row.second; bgg.get();
	    
	    getline ( bgg, row.curr, ',' );
	    
		bgg >> row.bid; bgg.get();
		bgg >> row.ask; bgg.get();
		
		row.time = row.hour * 3600 + row.minute * 60 + row.second;
		
//		row = make_tuple(time, curr, bid, ask);
	    
	    cout << row.time << " x ";
	    cout << row.curr << " x ";
	    cout << row.bid << " x ";
	    cout << row.ask << " x ";

		cout << " : " << i << endl;
//		cout<< row << " : " << i << endl;
		i++;
		
		bgg_data.push_back(row);
//	     cout << string( value, 0, value.length() ); // display value removing the first and the last character from it
	}
	
	cout << bgg_data.size();
}
