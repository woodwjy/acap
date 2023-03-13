#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <dlfcn.h>
#include <errno.h>
#include <dirent.h>
#include "module.h"


int load_modules()
{
    void *flib;
    int (*pfunc)();
    char *error_message;

    char path[FILE_DIR_LEN]=APTE_MODULE_PATH;
    DIR *dir = opendir(APTE_MODULE_PATH);
    if(dir == NULL) 
        perror("[apte]we cannot open the path\n");

    struct dirent *ent;
    int i=0;
    int result = 0;


    while((ent=readdir(dir)) != NULL)
    {
        if(!strcmp(ent->d_name,".")||!strncmp(ent->d_name,"..",2))
            continue;

        if(!strstr(ent->d_name,".so"))
            continue;

        struct stat st;
        char *name = (char *)malloc(sizeof(char)*FILE_DIR_LEN);
        memset(name,'\0',sizeof(char)*FILE_DIR_LEN);
        strcpy(name,APTE_MODULE_PATH);
        strcat(name,"/");
        strcat(name,ent->d_name);

        int f=stat(name,&st);
        if(f == -1)
        {
            switch(errno)
            {
                case EACCES: puts("EACCES");break;
                case EBADF: puts("EBADF");break;
                case EFAULT: puts("EFAULT");break;
                case ENOENT: puts("ENOENT");break;
                case ENOMEM: puts("ENOMEM");break;
                case ENOTDIR: puts("ENOTDIR");break;  
            }
        }

        if(!S_ISDIR(st.st_mode)) 
        {
            sprintf(path,"%s%s",APTE_MODULE_PATH,ent->d_name);
            flib = dlopen(path, RTLD_LAZY);
            error_message = dlerror();

            if(error_message)
            {   
                printf("[apte]%s\n", error_message);
                printf("[apte]cannot load %s\n", path);
                //return (-1);
            }
            

            pfunc = dlsym(flib, "module_init");
            error_message = dlerror();
            if(error_message)
            {
                printf("[apte]cannot open the function func\n");
                //return (-1);
            }else{
                result = pfunc();
                printf("[apte]%s load result is: %d\n",path, result);
            }
        }
            
    }
    return 0;
}
