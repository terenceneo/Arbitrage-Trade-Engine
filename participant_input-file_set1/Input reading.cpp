#include <bits/stdc++.h>

#define PORT 8080

using namespace std;

struct row_t {
	string curr;
	int hour, minute, second;
	long long time;
	double bid, ask;
	string all; //time, bid, ask stored as a string
};

list<row_t> bgg_data, ebs_data, reu_data; // all rows from provider

bool data_ready (list<row_t> &data, int time) {
	return data.front().time >= time;
}

string get_next (list<row_t> &data) { //check that data is not empty before calling this function
	string msg = data.front().all;
	data.pop_front();
	return msg;
}

int main() {
	row_t row;

	vector<string> csvs { "bbg.csv", "ebs.csv", "reu.csv" };

//	cerr << csvs.size() << endl;
	
//	int i = 0;
	
	for (string csv : csvs) {
		ifstream data (csv); // declare file stream
		
		while ( data.good() ){		
			//taking in row data
			data >> row.hour; data.get();
			data >> row.minute; data.get();
			data >> row.second; data.get();
		    
		    getline ( data, row.curr, ',' );
		    
			data >> row.bid; data.get();
			data >> row.ask; data.get();
			
			string provider = csv.substr(0,3);
			row.time = row.hour * 3600 + row.minute * 60 + row.second;
			row.all = provider + " " + to_string(row.time) + " " + to_string(row.bid) + " " + to_string(row.ask);
			
	//	    // printing input for debugging
	//	    cerr << row.time << " x ";
	//	    cerr << row.curr << " x ";
	//	    cerr << row.bid << " x ";
	//	    cerr << row.ask << " x ";
	//
	//		cerr << " : " << i << endl;
	//		i++;
			
			if (provider == "bbg") bgg_data.push_back(row); // can be thrown away
			else if (provider == "ebs") ebs_data.push_back(row);
			else if (provider == "reu") reu_data.push_back(row);	
		}
		
	//	cerr << bgg_data.size();
	}
	
	// sending over data rows
	while (!bgg_data.empty() && !ebs_data.empty() && !reu_data.empty()) {
		string msg;
		int curr_time = 0;
		if (data_ready(bgg_data, curr_time)) {
			msg = get_next(bgg_data);
			cerr << msg << endl;
		} else if(data_ready(ebs_data, curr_time)) {
			msg = get_next(ebs_data);
			cerr << msg << endl;
		} else if (data_ready(reu_data, curr_time)) {
			msg = get_next(reu_data);
			cerr << msg << endl;
		}
	}
		
}
