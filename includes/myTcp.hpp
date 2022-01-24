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
#include <mutex>

using namespace std;

#define NoOfNode 4

class manyToMany
{
    mutex m;
    int serv_sock;
    vector<int> clnt_sock; // 클래스 생성시 N-1개의 소켓 생성
    vector<int> connect_clnt_sock; // 실제 연결된 소켓 리스트
    int connect_cnt = 0;
    vector<in_addr_t> connected_clnt_addr_list; // 실제 연결된 주소 리스트
    string my_ip;
    const char* port = "40201";
    string server_ip[4] = {"192.168.0.100", "192.168.0.101", "192.168.0.102", "192.168.0.103"};

    struct sockaddr_in serv_addr;
    //struct sockaddr_in clnt_addr;
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
    bool crea_conn_sock(int sock, in_addr_t ip);
    
};
