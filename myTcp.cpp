#include "myTcp.hpp"



bool manyToMany::has_ip(in_addr_t ip)
{
    for (int i=0; i< connected_clnt_addr_list.size(); i++)
    {
        if(connected_clnt_addr_list[i] == ip)
            return true;
    }
    return false;
}

manyToMany::manyToMany(string my_ip)
{
    this->my_ip = my_ip;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(port));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handring("bind() error");

    /*
    client socket 생성
    */
    for (int i = 0; i < NoOfNode-1; i++)
    {
        clnt_sock.push_back(socket(PF_INET, SOCK_STREAM, 0));

        if (clnt_sock.back() == -1)
            error_handring("socket() error\n");
    }

    /*
    write(clnt_sock, message, sizeof(messages));
    */

   
}

manyToMany::~manyToMany()
{
    printf("모든 과정 종료합니다...\n");
    close(serv_sock);
    for (int i = 0; i < clnt_sock.size(); i++)
        close(clnt_sock[i]);

    printf("생성된 모든 소켓은 %lu개 입니다.\n", connect_clnt_sock.size());
}

void manyToMany::error_handring(string message)
{
    cout << message << endl;
    exit(1);
}

void manyToMany::server()
{
    thread server_t(&manyToMany::server_run, this);
    cout << "server() run" << endl;
    server_t.detach();
    //server_t.join();
}
void manyToMany::server_run()
{
    if (listen(serv_sock, 5) == -1)
        error_handring("listen() error");

    while (true)
    {
        clnt_addr_size[connect_cnt] = sizeof(clnt_addr[connect_cnt]);
        clnt_sock.push_back(accept(serv_sock, (struct sockaddr *)&clnt_addr[connect_cnt], &clnt_addr_size[connect_cnt]));

        if (clnt_sock.back() == -1)
            error_handring("accept() error");
        else if (has_ip(clnt_addr[connect_cnt].sin_addr.s_addr) == true){
            clnt_sock.pop_back();
            continue;
        }
        crea_conn_sock(clnt_sock.back());
        //connect_clnt_sock.push_back(clnt_sock.back());
        connected_clnt_addr_list.push_back(clnt_addr[connect_cnt].sin_addr.s_addr);
        cout << "서버 :" << clnt_addr[connect_cnt].sin_addr.s_addr << " 연결됨" << endl;
        connect_cnt++;
        sleep(0.05);
    }
}


void manyToMany::client(int number_of_client)
{
    vector<thread> client_t;
    printf("%d 개의 클라이언트를 생성합니다\n", number_of_client-1);

    for (int i = 0; i < number_of_client; i++)
    {
        client_t.push_back(thread(&manyToMany::client_run, this, i));
        client_t[i].detach();
        sleep(0.07);
    }

    
    //for (int i = 0; i < number_of_client; i++)
}

void manyToMany::client_run(int index)
{
    struct sockaddr_in addr;
    string ip = server_ip[index];

    if (my_ip == ip)
        return;
    in_addr_t int_ip = inet_addr(ip.c_str());
    //printf("%u\n", int_ip);
    
    
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = int_ip;
    
    
    //printf("%d\n", connect_cnt);
    addr.sin_port = htons(atoi(port));

    bool connected;
    
    printf("%s 에 %d 포트로 연결 시도합니다...\n", ip.c_str(), addr.sin_port);
    for (int i = 0; i < 10; i++)
    {
        if (has_ip(int_ip))
            return;
        connected = false;
        
        if (connect(clnt_sock[index], (struct sockaddr *)&addr, sizeof(addr)) != -1){
            connected = true;
            crea_conn_sock(clnt_sock[index]);
            //connect_clnt_sock.push_back(clnt_sock[index]);
            connect_cnt++;
            break;
        }
        sleep(1);
        if(has_ip(int_ip))
            break;
    }

    if(connected)
        cout << ip << " 연결 성공!!\n";
    else
        cout << ip << " 연결 실패..\n";

    
    
    printf("소켓 수 : %d\n", clnt_sock.size());
    
}

void manyToMany::send_msg(char* msg)
{
    for(int i=0; i<connect_clnt_sock.size(); i++)
    {
        write(connect_clnt_sock[i], msg, strlen(msg));
    }
}
void manyToMany::crea_conn_sock(int sock)
{
    connect_clnt_sock.push_back(sock);
    thread t(&manyToMany::recv_msg, this, sock);
    t.detach();
}

void manyToMany::recv_msg(int sock)
{
    printf("recieve socket : %d\n", sock);
    int read_len;
    char message[100];
    printf("%d 에 대한 recv 진행..\n", sock);
    while(read_len=read(sock, message, sizeof(message)))
    {
        if(read_len==-1){
            printf("socket : %u에서 에러 발생\n", sock);
            return;
        }
        
        if(strcmp(message, "exit") == 0)
        {
            printf("종료 메시지 수신. 종료합니다...\n");
            exit(0);
        }

        printf("%s\n", message);
    }
}