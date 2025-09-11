#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>

// int main() {
//     system("echo 'Hello from system()!'");
//     return 0;
// }


// int main()
// {
//     // fprintf(stderr, "oops: %s\n", "no file");
//     // printf("oops: %s\n", "no file");

//     if (chdir("/User/leia/desktop/out.md/hey") == -1) 
//     { //如果更改目录失败
//     perror("cd");  // 输出: cd: No such file or directory
//     }

    
// }

// #include <unistd.h>
// #include <stdio.h>
// #include <errno.h>

// int main(void) {
//     if (chdir("/tmp//tmp/myfile.txt/somewhere") != 0) {
//         perror("cd");
//         return 1;
//     }
//     return 0;
// }

#include<stdio.h>
#include<stdlib.h> // for getenv setenv
// extern char** environ;
int main(int argc, char* argv[]){
    printf("Start...\n");
    fflush(stdout);
    char* path = getenv("PATH");
    printf("$PATH: %s\n", path);
    fflush(stdout);
    setenv("HELLO", "WORLD", 1);
    char* hello = getenv("HELLO");
    printf("$HELLO: %s\n", hello);
    fflush(stdout);
    printf("End\n");
    fflush(stdout);
    return 0;
}