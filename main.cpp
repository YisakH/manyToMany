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
    return 0;
}