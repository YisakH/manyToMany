#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <vector>
#include <map>

using namespace std;

class manyToMany
{
    int serv_sock;
    int clnt_sock[4];
    int connect_cnt = 0;
    vector<in_addr_t> clnt_vec;
    string my_ip;
    const char* port = "40201";
    string server_ip[4] = {"192.168.0.100", "192.168.0.101", "192.168.0.102", "192.168.0.103"};

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr[4];
    socklen_t clnt_addr_size[4];



public:
    manyToMany(std::string ip);
    ~manyToMany();

    void error_handring(string message);

    void server();
    bool has_ip(in_addr_t ip);
    void server_run();
    void client(int number_of_client);
    void client_run(string ip);
};