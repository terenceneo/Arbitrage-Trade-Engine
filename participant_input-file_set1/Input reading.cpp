#include <bits/stdc++.h>

using namespace std;

struct row_t {
	string curr;
	int hour, minute, second;
	long long time;
	double bid, ask;
	string all; //time, bid, ask stored as a string
};

vector<row_t> bgg_data, ebs_data, reu_data; // all rows from provider

int main() {
	row_t row;
	
//	ifstream bgg ( "bbg.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
//	ifstream ebs ( "ebs.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
//	ifstream reu ( "reu.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	vector<string> csvs { "bbg.csv", "ebs.csv", "reu.csv" };
	 
//	cerr << csvs.size() << endl;
	
//	int i = 0;
	
	for (string csv : csvs) {
		ifstream data (csv);
		while ( data.good() ){		
			//taking in row data
			data >> row.hour; data.get();
			data >> row.minute; data.get();
			data >> row.second; data.get();
		    
		    getline ( data, row.curr, ',' );
		    
			data >> row.bid; data.get();
			data >> row.ask; data.get();
			
			row.time = row.hour * 3600 + row.minute * 60 + row.second;
			row.all = csv + ", " + to_string(row.time) + ", " + to_string(row.bid) + ", " + to_string(row.ask);
			
	//	    // printing input for debugging
	//	    cerr << row.time << " x ";
	//	    cerr << row.curr << " x ";
	//	    cerr << row.bid << " x ";
	//	    cerr << row.ask << " x ";
	//
	//		cerr << " : " << i << endl;
	//		i++;
			
			cerr << row.all << endl;
			
			bgg_data.push_back(row); // can be thrown away
		}
		
	//	cerr << bgg_data.size();
	}
}
