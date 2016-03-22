#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"rw.h"
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
#define MAX_LEN 10240
/******************* send a single file **********************/
int  send_file(int sockfd ,char *file_name)
{
   int    file_fd;
   int    send_file_size;
   int    send_read_left;
   int    read_flag;
   int    write_flag;
   int    send_buff[MAX_LEN];
   int    total = 0;
   int    len;
   char   size_buf[128];
   struct stat send_file_state;
   memset(size_buf,0,128);
   if ((file_fd = open(file_name,O_RDONLY)) < 0) {
   printf("open file:%s fail\n",file_name);
   exit(0);
   }  
   if (fstat(file_fd,&send_file_state) == -1) {
   printf("get info faile\n");
   exit(0);
   }
   send_file_size = send_file_state.st_size;
   send_read_left = send_file_size;
   len            = MAX_LEN;
   itoa(send_file_size,size_buf);
   if (write(sockfd,size_buf,strlen(size_buf)) < 0) {
   printf("write wrong\n");
   }

   while (send_read_left > 0)
   {
   memset(send_buff,0,MAX_LEN);
   if (send_read_left < MAX_LEN) {
   len = send_read_left;
   printf("send the last:%d\n",len);   
   }else{
   len = MAX_LEN;
   }
   if ((read_flag=read(file_fd,send_buff,len)) < 0) {
   perror("read");
   break;
   }
// printf("read:%d\n",len);
//((error_flag = read(sockfd,recvbuf,len))==0)
   if ((write_flag=write(sockfd,send_buff,len)) < 0) {
   perror("write");
   break;
   }
   send_read_left -= len;
   printf("write:%d\n",len);
   total += len;
   }
   printf("send:%d\n",total);
   close(file_fd);
}
/*************** recv a single file ******************/

int recv_file(int sockfd,char *file_name)
{
   int    file_fd;
   int    recv_read_left;
   int    recv_file_size;
   int    read_flag;
   int    write_flag;
   int    recv_buff[MAX_LEN];
   int    nread = 0;
   int    len;
   int    total_len=0;
 //  struct stat recv_file_state;
   char   size_buff[128]
   memset(size_buff,0,128);
   if ((nread=read(sockfd,size_buff,sizeof(size_buff))) < 0) {
   printf("read wrong\n");
   exit(0);
   }
  // recv_file_size = recv_file_state.st_size;
   recv_read_left = atoi(size_buff);

   if((file_fd = open(file_name,O_CREAT|O_RDWR,S_IRUSR|S_IRGRP|S_IROTH)) == -1)
   {printf("open failed %s\n",file_name);
   exit(0);
   }

   while (recv_read_left > 0)
   {
   memset(recv_buff,0,MAX_LEN);
   if (recv_read_left < MAX_LEN) { 
   len = recv_read_left;
   printf("The last pakge!\n");
   }else {
   len=MAX_LEN;
   }

   if ((read_flag = read(sockfd,recv_buff,len)) < 0) {
   printf("read:wrong!\n");// later change
   break;
   }
   printf("read:%d\n",len);
   if (len == 0) { 
   printf("\nreceive all pakeg\n");
   break;
   }
   printf("read length is %d\n",len);
   if(write_flag=write(file_fd,recv_buff,len) == -1) 
    {
     printf("\nwrite:worng\n");
     break;
    }
   printf("write:%d\n",len);
   recv_read_left -= len;
   total_len += len;
   }
   printf("\nrecevied %d bytes\n",total_len);
   close(file_fd);
}

 int list_all(int sockfd,char *file_dir,int size)
{
   char   mother[512];
   char   temp_buff[1024];
   char   message[512];          
   char   buff[512];
   char   mark_end[16]="end"; 
   int    i=0;
   int    count=0;
   int    filesize;
   char   *p0;
   int    size_s=size;
   struct list
   {
   char   pathname[1024];
   char   filename[512];     
   struct list *next;
   };
 
   p0  = file_dir;              
   memset(mother,'*',512);
   struct list *head,*p1,*p2;

   void scan_dir(char *dir)          
   {
      DIR    *dp;
      struct dirent  *entry;
      struct stat    statbuff;
   
      if(!(dp=opendir(dir)))
      {
      printf("can't open:%s\n",dir);
      exit(0);
      }
      chdir(dir); 
      while((entry=readdir(dp))!=NULL)
      {
         lstat(entry->d_name,&statbuff); 
         if(S_IFDIR&statbuff.st_mode) 
         {
         if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name)== 0)
         continue;
         scan_dir(entry->d_name);  
         }else{
         char path_buff[MAXPATH];
         getcwd(path_buff, MAXPATH);
         p1= malloc(sizeof(struct list));
         strcpy(p1->pathname,path_buff);
         strcpy(p1->filename,entry->d_name);
         p1->next=0;
         count++;
         if(count==1)
         head=p2=p1;
         else
         {p2->next=p1;
          p2=p1;
         }
         int tem;
         tem = statbuff.st_size;
         filesize += tem;
         }
      }
   chdir("..");                //back up
   closedir(dp);
   }
   
   
   int print(int sock_fd,int size_ss)
   {
      struct list *temp;
      char   *str=NULL;
      char   *p3=NULL;
      char   *p4=NULL;
      char   cpy[1024];
      temp=head;
      if(head!=NULL)
      while(1)
      {
      if(temp==NULL)
      {
      write(sock_fd,mark_end,sizeof(mark_end));
      break;
      }
      str = strcat(temp->pathname,"/");
      memset(temp_buff,0,1024);
      memset(message,0,512);
      memset(cpy,0,1024);
      strcat(temp_buff,temp->pathname);
      strcat(temp_buff,temp->filename);
      p3=temp_buff;
      p4 = p3+size_ss;
      strcpy(cpy,p4);
      strcat(cpy,mother);
      snprintf(message,512,"%s",cpy);
      if(write(sock_fd,message,sizeof(message)) < 0)
      printf("write dir wrong!\n");
    
    //  printf("%s\n",message);
      temp=temp->next;
      }
   }
 scan_dir(p0);
 print(sockfd,size_s);
 free(p1);
}



int recv_list_all(int sockfd)
{
  char message_buff[512];
  char buff[512];
  while(1)
  {
     int i=0;
     memset(message_buff,0,512);
     read(sockfd,message_buff,sizeof(message_buff));
   //  printf("read message_buff:%s\n",message_buff);
     if(strlen(message_buff) < 4)
     break;
     memset(buff,0,512);
     while(1)
     {
     if(message_buff[i]!= '*')
       { 
        buff[i]=message_buff[i];
         i++;
      }else{
            break;
           }
     }
    printf("%s\n",buff);
  }
}
