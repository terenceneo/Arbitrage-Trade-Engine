// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#define PORT 8080

struct Quote {
    long long start_time, expiry_time;
    string exchange, cp;
    double bid, ask;
};

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    /*
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    
    double net_profit = 0;
    
    
    vector<Quote> quote_list;
    
    int counter = 101;
    while (counter--) {
        
        long long latest;
        
        // the following loop takes in 3 sets of data.
        for (int i = 0; i < 3; ++i) {
            Quote current;
            
            valread = read(new_socket, buffer, 1024);
            string input(buffer);
        
//            cout << input;
        
            stringstream ss(input);
        
            ss >> current.exchange >> current.start_time >> current.bid >> current.ask;
        
            current.expiry_time = current.start_time + (current.exchange == "reu" ? 3 : current.exchange == "ebs" ? 4 : 5);
        
            quote_list.push_back(current);
            
            latest = current.start_time;
        }
        
        int trades = 6;
        
        bool profitable = true;
        
        while (profitable) {
            profitable = false;
            
            // find max bid
            double max_bid = 0;
            int max_pos = 0;
            for (int i = 0; i < quote_list.size(); ++i) {
                Quote current = quote_list[i];
                if (current.bid > max_bid) {
                    max_bid = current.bid;
                    max_pos = i;
                }
            }
                
            // find min ask
            // ASSUMPTION: from the same quote, bid is strictly < ask.
            double min_ask = 2e9;
            int min_pos = 0;
            for (int i = 0; i < quote_list.size(); ++i) {
                Quote current = quote_list[i];
                if (current.ask < min_ask) {
                    min_ask = current.ask;
                    min_pos = i;
                }
            }
                
            // check for profitability
            if (max_bid - min_ask > 0) {
                // profitable!
                profitable = true;
                net_profit += max_bid - min_ask;
                
                printf("Profit for this transaction: %lf\n\n", max_bid - min_ask);
                
                string b_request;
                b_request = "EURUSD B ";
                b_request += quote_list[min_pos].exchange;
                b_request += " ";
                b_request += quote_list[min_pos].bid;
                b_request += " ";
                b_request += quote_list[min_pos].start_time;
                b_request += " ";
                b_request += quote_list[min_pos].expiry_time;
                send(new_socket, b_request.c_str(), strlen(b_request.c_str()), 0);
                
                string s_request;
                s_request = "EURUSD S ";
                s_request += quote_list[max_pos].exchange;
                s_request += " ";
                s_request += quote_list[max_pos].bid;
                s_request += " ";
                s_request += quote_list[min_pos].start_time;
                s_request += " ";
                s_request += quote_list[min_pos].expiry_time;
                send(new_socket, s_request.c_str(), strlen(s_request.c_str()), 0);
                
                quote_list[max_pos].bid = 0;
                quote_list[min_pos].ask = 2e9;
                
                --trades;
            }
            
            // not profitable
            while (trades-- >= 0) {
                string fallback = "emptystring emptystring emptystring 0 1 2";
                send(new_socket, fallback.c_str(), strlen(fallback.c_str()), 0);
                send(new_socket, fallback.c_str(), strlen(fallback.c_str()), 0);
            }
            
            trades = 6;
            
         }
        
        for (int i = 0; i < quote_list.size(); ++i) if (quote_list[i].expiry_time <= latest + 1) quote_list.erase(quote_list.begin() + i);
        
         printf("%s\n", buffer);
    }
    
    printf("Total profit: %lf", net_profit);
    
    /*
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
     
     
     */
    return 0;
}
