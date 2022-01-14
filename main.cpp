#include "includes/myTcp.hpp"

int main(int argc, char *argv[])
{
    if(argc <= 1){
        printf("argument must be required\n");
        exit(1);
    }

    manyToMany mymanyToMany = manyToMany(argv[1]);
    printf("%s\n", argv[1]);

    mymanyToMany.server();
    sleep(0.1);
    //mymanyToMany.server();
    mymanyToMany.client(4);

    string input = "";


    sleep(2);
    printf("자고 일어났어요\n");
    mymanyToMany.run_recv_t();

    while(input.compare("exit") != 0){
        cin >> input;
        cout << input << endl;
    }
    return 0;
}