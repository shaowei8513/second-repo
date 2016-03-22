#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<dirent.h>
#include"rw.h"

int main()
{
    int     sockfd;
    int     conn_ret;
    int     mark;
    struct  sockaddr_in servaddr;
    char    cmd_buff[MAX_LEN];
    char    recvbuff[MAX_LEN];
    size_t  error_flag;
    size_t  len = MAX_LEN;
    size_t  read_left;
    int     file_fd;
    int     total_len = 0;
  /************* create a socket bind a server **************/ 
  if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
   {
   perror("sock");
   exit(1); 
   }
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(9000);
   inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
   conn_ret = connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
   if(conn_ret == -1)
   {
   perror("connect");
   exit(0);
   } 
  /************* client login **************/ 
   while(1) 
   { 
   char a[16];
   if(client_login(sockfd) ==  0)
   {mark=1;
   break;
   }else {
   printf("client_login_failed!\n");
   } 
   printf("write (r) relogin or write (q) quite\nserver>");
   scanf("%s",a);
   if(strcmp(a,"r")==0)
   continue;
   if(strcmp(a,"q")==0)
   { mark=0;
   break;
   }
   /************* input cmd to send server and excuted **************/               
   }
   while(mark)
   {
   int     cmd_int;
   int     size;
   struct  cmd client_mems;
   struct  stat client_stat_buf;
   char    *cmd_file_name = NULL;
   char    cmd_buf[512];
   memset(&client_mems,0,sizeof(client_mems));
   memset(cmd_buf,0,512);   
   printf("\nserver>");
   fgets(cmd_buf,512,stdin);
//   scanf("%s %s %s",client_mems.cmd,client_mems.server_cmd,client_mems.client_cmd);
   if(strcmp(client_mems.cmd,"quit")==0)
   {
   write(sockfd,&client_mems,sizeof(struct cmd));
   break;       
   }
   if(strcmp(client_mems.cmd,"up")==0)
   {
   if(access(client_mems.client_cmd,F_OK)==0)
   lstat(client_mems.client_cmd,&client_stat_buf);
        
    if (S_IFDIR&client_stat_buf.st_mode)
    {
    strcpy(client_mems.cmd,"4");
    cmd_int=4;
    cmd_file_name=client_mems.client_cmd;
    write(sockfd,&client_mems,sizeof(struct cmd));
    }else{
    strcpy(client_mems.cmd,"2");
    cmd_int=2;
    cmd_file_name=client_mems.client_cmd;
    write(sockfd,&client_mems,sizeof(struct cmd));
    }
    }
   if(strcmp(client_mems.cmd,"down")==0)
   {
   if(access(client_mems.client_cmd,F_OK)!=0);
   printf("the fiel exist,cover it \n");

   write(sockfd,&client_mems,sizeof(struct cmd));
   memset(&client_mems,0,sizeof(client_mems));
            
   read(sockfd,&client_mems,sizeof(struct cmd));
   cmd_int = atoi(client_mems.cmd);
   cmd_file_name=client_mems.client_cmd;
   size=strlen(client_mems.server_cmd);
   }
   if(strcmp(client_mems.cmd,"rmdir")==0)
   {
   cmd_int = 7;
   write(sockfd,&client_mems,sizeof(struct cmd));
   }
   if(strcmp(client_mems.cmd,"mkdir")==0)
   {
   cmd_int = 6;
   write(sockfd,&client_mems,sizeof(struct cmd));
   }
   if(strcmp(client_mems.cmd,"ls")==0)
   {
   cmd_int=5;
   write(sockfd,&client_mems,sizeof(struct cmd));     
   }
  /************* excute the different cmd **************/ 
   switch(cmd_int)
   {
     case 1:
       recv_file(sockfd,cmd_file_name);
       break;
     case 2:
       send_file(sockfd,cmd_file_name);
       break;
     case 3:
       recv_ser_dir_file(sockfd,cmd_file_name,size);
       break;
     case 4:
       send_dir_file(sockfd,cmd_file_name);
       break;
     case 5:
       recv_list_all(sockfd);
       break;
     case 6:
       printf("mkdir over\n");
       break;
     case 7:
       printf("rmdir over\n");
       break;
     default:
       printf("cmd wrong\n");
       break;
   }

 }
close(sockfd);
exit(0);
}
