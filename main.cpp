#include "includes/myTcp.hpp"

int main(int argc, char *argv[])
{
    if(argc <= 1){
        printf("argument must be required\n");
        exit(1);
    }

    manyToMany mymanyToMany(argv[1]);
    printf("%s\n", argv[1]);

    mymanyToMany.server();
    sleep(0.1);
    mymanyToMany.client(NoOfNode);


    string input = "";


    sleep(2);

    while(true){
        getline(cin, input);
        mymanyToMany.send_msg((char *)input.c_str());
    }

    return 0;
}