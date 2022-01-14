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

#define NoOfNode 2

class manyToMany
{
    int serv_sock;
    vector<int> clnt_sock;
    vector<int> connect_clnt_sock;
    int connect_cnt = 0;
    vector<in_addr_t> connected_clnt_addr_list;
    string my_ip;
    const char* port = "40201";
    string server_ip[3] = {"192.168.0.23", "192.168.0.9", "192.168.0.10"};

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr[NoOfNode];
    socklen_t clnt_addr_size[NoOfNode];



public:
    manyToMany(std::string ip);
    ~manyToMany();

    void error_handring(string message);

    void server();
    bool has_ip(in_addr_t ip);
    void server_run();
    void client(int number_of_client);
    void client_run(int index);
    void send_msg(char* msg);
    void recv_msg(int sock);
    void crea_conn_sock(int sock);
    
};