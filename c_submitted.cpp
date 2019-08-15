
//#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <list>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <unistd.h>

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
    return data.front().time <= time;
}

string get_next (list<row_t> &data) { //check that data is not empty before calling this function
    string msg = data.front().all;
    data.pop_front();
    return msg;
}

int main(int argc, char const *argv[]) {
    //Setting up client
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    
    // Processing data
    row_t row;
    
    
    vector<string> csvs;
    csvs.push_back("bbg.csv");
    csvs.push_back("ebs.csv");
    csvs.push_back("reu.csv");

    
    //    cerr << csvs.size() << endl;
    
    //    int i = 0;
    
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
            
            //        // printing input for debugging
            //        cerr << row.time << " x ";
            //        cerr << row.curr << " x ";
            //        cerr << row.bid << " x ";
            //        cerr << row.ask << " x ";
            //
            //        cerr << " : " << i << endl;
            //        i++;
            
            if (provider == "bbg") bgg_data.push_back(row); // can be thrown away
            else if (provider == "ebs") ebs_data.push_back(row);
            else if (provider == "reu") reu_data.push_back(row);
        }
        
        //    cerr << bgg_data.size();
    }
    
    // sending over data rows
    
    // start time
    auto start = chrono::steady_clock::now();
    
    while (!bgg_data.empty() && !ebs_data.empty() && !reu_data.empty()) {
        string msg;
        auto end = chrono::steady_clock::now();
        long long curr_time = chrono::duration_cast<chrono::seconds>(end - start).count();
        //        cerr << curr_time << endl;
        if (data_ready(bgg_data, curr_time)) {
            msg = get_next(bgg_data);
            cerr << msg << endl;
            send(sock , msg.c_str(), strlen(msg.c_str()) , 0 );
            cerr << "confirmation message\n";
            
        } if(data_ready(ebs_data, curr_time)) {
            msg = get_next(ebs_data);
            cerr << msg << endl;
            send(sock , msg.c_str(), strlen(msg.c_str()) , 0 );
            cerr << "confirmation message\n";
            
        } if (data_ready(reu_data, curr_time)) {
            msg = get_next(reu_data);
            cerr << msg << endl;
            send(sock , msg.c_str(), strlen(msg.c_str()) , 0 );
            cerr << "confirmation message\n";
        }
    }
    
    
    
    valread = read(sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
