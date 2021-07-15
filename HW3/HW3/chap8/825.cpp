#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    unsigned int cluster = atoi(argv[1]);
    unsigned int page = cluster>>12;
    unsigned int offset = cluster & 8191;
    
    printf("page number = %d\noffset = %d\n",page,offset);
    return 0;
}
