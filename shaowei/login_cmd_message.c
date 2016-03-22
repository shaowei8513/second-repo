#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<dirent.h>
#include"rw.h"
#include<sys/stat.h>
#include<fcntl.h>

char *server_login(int sockfd)
{ 
   char   right[128]= "wellcome come back:";
   char   wrong[128]= "login wrong:";
   char   *puser=NULL;
   char   *pkey=NULL;
   char   cmd_buff[MAX_LEN];
   int    len = MAX_LEN;
   struct login server_login;

   read(sockfd,&server_login,sizeof(struct login));
   printf("struct user:%s\n",server_login.user);
   
   printf("struct key:%s\n",server_login.key);
   if((puser= compare(server_login.user,"/shaowei/user_list"))==NULL)
   strcat(wrong,server_login.user);
   printf("pin user:%s\n",puser); 
   if((pkey=compare(server_login.key,"/shaowei/user_list"))==NULL)
   strcat(wrong,server_login.key);
   printf("pin key:%s\n",pkey);
   printf("pin key:%d\n",sizeof(pkey));

   if(pkey!=NULL)
   printf("lalalala:%s\n",pkey);

   if((puser!=NULL)&&(pkey!=NULL))
   { 
   printf("right\n");
   printf("user:%s\n",puser);
   printf("key:%s\n",pkey);
   strcat(right,puser);
   write(sockfd,right,128);
   printf("message:%s\n",right);
   } else{ 
       puser=NULL;
       printf("wrong:\n");
       write(sockfd,wrong,128);
       }
   return puser;
   free(puser);
   free(pkey);
}

int client_login(int sockfd)
{
    char   cmd_buff[128];
    char   user_buff[128];
    char   key_buff[128];
    int    len = 128;
    char   max[1024]="wellcome come back:";
    struct login client_login;
 
    memset(cmd_buff,0,128);
    memset(user_buff,0,128);
    //printf("%s\n",cmd_buff);
    while(1)
    {
     printf("login:");
     scanf("%s",client_login.user);
     printf("passwd:");
     scanf("%s",client_login.key);

     write(sockfd,&client_login,sizeof(struct login));
    //  sleep(1);
     memset(cmd_buff,0,len);
     read(sockfd,cmd_buff,len);
     printf("server:%s\n",cmd_buff);
     break;
    }
    if(strcmp(strcat(max,client_login.user),cmd_buff)==0)
    { return 0;}else{return -1;}
}
