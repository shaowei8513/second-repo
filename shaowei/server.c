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
#include<sys/stat.h>
#include<dirent.h>
#define BACKLOG 10
int main()
{
  int        sockfd;
  int        connfd;
  char       file_name[MAX_LEN];
  char       cmd_buff[MAX_LEN];
  int        len = MAX_LEN;
  struct     sockaddr_in servaddr;
  struct     sockaddr_in cliaddr;
  struct     sockaddr_in tempaddr;
  char port[512];
  socklen_t  templen;
  socklen_t  clilen;

  /**************creat a socket to bind client************/

  if((sockfd =socket(AF_INET,SOCK_STREAM,0))==-1)
  {
  perror("socket");
  exit(1);
  }
  find_value("/shaowei/config","port",port);
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(atoi(port)); 
  if (bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
  {
  perror("bind");
  exit(1);
  } 
  if (listen(sockfd,BACKLOG) == -1)
  {
  perror("listen");
  exit(1);
  }
  
  /***********watting for client to connect*********** */

  while(1)
  {  
    if ((connfd = accept(sockfd,(struct sockaddr *)NULL,NULL)) == -1)
    {
    perror("accept");
    exit(0); 
    }
     /************** user login ****************/
  
    while(1)
    {
    memset(port,0,512);
    char local_work_path[512];
    char *temp_work_path=NULL; 
    if((temp_work_path=server_login(connfd))!=NULL)
    {
    printf("0000:%s\n",temp_work_path);
    }else{
    continue;
    }
    find_value("/shaowei/config",temp_work_path,port);
    printf("local_work_path:%s\n",port);
    break;
    }
    /************** get client cmd and executed ***************/
    while(1)
    {
    int cmd_int;
    int size;
    char portcpy[512];
    struct cmd server_mems;
    struct stat server_stat_buf;
    char cmd_all_name[1024];
    memset(&server_mems,0,sizeof(server_mems)); 
    memset(portcpy,0,1024);
    printf("waite new cmd\n");
               
    read(connfd,&server_mems,sizeof(struct cmd));
    printf("cmd:%s\n",server_mems.cmd);
    if(strcmp(server_mems.cmd,"quit")==0)
    { 
    close(connfd);
    break;
    }
    printf("connfd valual:%d\n",connfd);
    printf("socket:%d\n",sockfd);
    strcpy(portcpy,port);
    printf("local_work place copy portcpy,:%s\n",portcpy);

    if(strcmp(server_mems.cmd,"2")==0)//recv file file message
    {
    cmd_int=atoi(server_mems.cmd);
    strcat(portcpy,server_mems.server_cmd);
    }

    if(strcmp(server_mems.cmd,"4")==0)//recv dir file message
    {
    cmd_int=atoi(server_mems.cmd);
    strcat(portcpy,server_mems.server_cmd);
    size=strlen(server_mems.client_cmd);
    }

    if(strcmp(server_mems.cmd,"down")==0)
    {
      strcat(portcpy,server_mems.server_cmd);
      if(access(portcpy,F_OK)==0)
      lstat(portcpy,&server_stat_buf);
 
      if (S_IFDIR&server_stat_buf.st_mode)
      { 
      strcpy(server_mems.cmd,"3");
      cmd_int=3;
      strcpy(server_mems.server_cmd,portcpy);
      write(connfd,&server_mems,sizeof(struct cmd));//send dir file message
      }else{
      strcpy(server_mems.cmd,"1");
      cmd_int=1;
      write(connfd,&server_mems,sizeof(struct cmd));
      }
    }
               
    if(strcmp(server_mems.cmd,"ls")==0)//list all
    {
    cmd_int=5;
    size = strlen(portcpy);
    strcat(portcpy,server_mems.server_cmd);
    }
    if(strcmp(server_mems.cmd,"rmdir")==0) //rm dir & file
    {
    cmd_int=6;
    strcat(portcpy,server_mems.server_cmd);
    }
    if(strcmp(server_mems.cmd,"mkdir")==0)// mkdir new empty dir
    {
    cmd_int=7;
    strcat(portcpy,server_mems.server_cmd);
    }

    printf("cmd_int:%d\n",cmd_int);
    printf("protcpy:%s\n",portcpy);
    switch(cmd_int)
    {
       case 1:
       send_file(connfd,portcpy);
       break;
       case 2:
       recv_file(connfd,portcpy);
       break;
       case 3:
       send_dir_file(connfd,portcpy);
       break;
       case 4:
       recv_ser_dir_file(connfd,portcpy,size);
       break;
       case 5:
       list_all(connfd,portcpy,size);
       break;
       case 6:
       rm_dir_file(portcpy);
       break;
       case 7:
       create_dir(portcpy);
       break;
      //  case 8:
          //    break;*/
       default:
       printf("wrong cmd\n");
           break;
    }
    }
  close(connfd);
  }
}
