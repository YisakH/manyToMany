#include "myTcp.hpp"

bool manyToMany::has_ip(in_addr_t ip)
{
    for (int i = 0; i < connected_clnt_addr_list.size(); i++)
    {
        if (connected_clnt_addr_list[i] == ip)
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
    for (int i = 0; i < NoOfNode - 1; i++)
    {
        clnt_sock.push_back(socket(PF_INET, SOCK_STREAM, 0));

        if (clnt_sock.back() == -1)
            error_handring("socket() error\n");
    }
}

manyToMany::~manyToMany()
{
    printf("모든 과정 종료합니다...\n");
    close(serv_sock);
    for (int i = 0; i < clnt_sock.size(); i++)
        close(clnt_sock[i]);

    printf("생성된 모든 소켓은 %lu개 입니다.\n", connect_clnt_sock.size());
}

void manyToMany::exit_call()
{
    printf("종료 메시지 수신...\n");

    close(serv_sock);
    for (int i = 0; i < clnt_sock.size(); i++)
        close(clnt_sock[i]);
    
    for (int j=0; j<connect_clnt_sock.size(); j++)
        close(connect_clnt_sock[j]);

    printf("생성된 모든 소켓은 %lu개 입니다.\n", connect_clnt_sock.size());

    exit(0);
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
    while (true)
    {
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size;

        if (listen(serv_sock, 5) == -1)
            error_handring("listen() error");

        clnt_addr_size = sizeof(clnt_addr);
        int tmp_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

        if (tmp_sock == -1)
            error_handring("accept() error");
        
        crea_conn_sock(tmp_sock, clnt_addr.sin_addr.s_addr);

        sleep(0.1);
    }
}

void manyToMany::client(int number_of_client)
{
    vector<thread> client_t;
    printf("%d 개의 클라이언트를 생성합니다\n", number_of_client - 1);

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

    addr.sin_port = htons(atoi(port));

    bool connected;

    printf("%s 에 %d 포트로 연결 시도합니다...\n", ip.c_str(), addr.sin_port);
    for (int i = 0; i < 100; i++)
    {

        if (connect(clnt_sock[index], (struct sockaddr *)&addr, sizeof(addr)) != -1)
        {
            crea_conn_sock(clnt_sock[index], int_ip);
            break;
        }
        sleep(0.2);
    }
}

void manyToMany::send_msg(char *msg)
{
    for (int i = 0; i < connect_clnt_sock.size(); i++)
    {
        write(connect_clnt_sock[i], msg, strlen(msg));
    }
    
    if(strcmp(msg, "exit") == 0)
        exit_call();
}

bool manyToMany::crea_conn_sock(int sock, in_addr_t ip)
{
    m.lock();

    if(has_ip(ip)){
        m.unlock();
        return false;
    }
    connected_clnt_addr_list.push_back(ip);
    connect_clnt_sock.push_back(sock);
    m.unlock();

    char str[30];
    printf("%s, sock : %u에 연결 성공!!\n", inet_ntop(AF_INET, &ip, str, sizeof(str)), sock);
    //printf("%s\n", str);

    thread t(&manyToMany::recv_msg, this, sock, str);
    t.detach();
    
    return true;
}

void manyToMany::recv_msg(int sock, char * ip)
{
    int read_len;
    char message[100];
    while (read_len = read(sock, message, sizeof(message)))
    {
        if (read_len == -1)
        {
            printf("socket : %u에서 에러 발생\n", sock);
            return;
        }

        if (strcmp(message, "exit") == 0)
        {
            exit_call();
        }

        printf("%s : %s\n", ip, message);
        memset(message, 0x00, sizeof(message));
    }
}