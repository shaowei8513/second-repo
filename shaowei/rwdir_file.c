#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include"rw.h"
#include<sys/stat.h>
#include<fcntl.h>

int send_dir_file(int sockfd,char *from_dir_name)
{
   char   mother[512];
   char   temp_buff[1024];
   char   message[512];          
   char   buff[512];
   char   mark_end[16]="end"; 
   int    i=0;
   int    count=0;
   int    filesize;
   char   *p0=NULL;
   struct list
   {
   char   pathname[1024];
   char   filename[512];     
   struct list *next;
   };
 
   p0  = from_dir_name;              
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
   
   
   int print(int sock_fd)
   {
      struct list *temp;
      char   *str;
      temp=head;
      if(head!=NULL)
      while(1) {
      if(temp==NULL)
      {
      write(sock_fd,mark_end,sizeof(mark_end));
      break;
      }
      str = strcat(temp->pathname,"/");
      memset(temp_buff,0,1024);
      memset(message,0,512);
      strcat(temp_buff,temp->pathname);
      strcat(temp_buff,temp->filename);
      strcat(temp_buff,mother);
      snprintf(message,512,"%s",temp_buff);
      if(write(sock_fd,message,sizeof(message)) < 0)
      printf("write dir wrong!\n");
    
      printf("%d,a%s\n",strlen(message),message);
      send_file(sock_fd,strcat(temp->pathname,temp->filename));
      printf("full name:%s\n",temp->pathname);
      temp=temp->next;
      }
   }

 scan_dir(p0);
 print(sockfd);
 free(p1);
}

int recv_dir_file(int sockfd,char *to_dir_name)
{
   char  message_buff[512];
   char  buff[512];
   char  temp[512];
   char  *file_name;
   char  file_path[1024];
   int   i=0;
   int   dn_size;

   while(1)
   {
   int i=0;
   memset(file_path,0,1024);
   memset(message_buff,0,512);
   memset(temp,0,512);
   read(sockfd,message_buff,sizeof(message_buff));
   
   if(strlen(message_buff) < 2)
   break;
   memset(buff,0,512);
   while(1)
   {
   if(message_buff[i]!='*')
   { 
   buff[i]=message_buff[i];
   i++;
   }else{
   break;
   }
   }
   strcpy(temp,to_dir_name);
   printf("%s\n",temp);
   strcat(temp,buff);
   printf("dir:%s\n",temp);
   dn_size = strlen(temp);
   while(1)
   {
   if(temp[dn_size--] == '/')
   {
   file_name=&temp[dn_size+2];
   break;
   }
   }
   printf("%d\n",(dn_size+2));
   snprintf(file_path,(dn_size+2),"%s",temp);
   printf("====%s\n",file_name);
   printf("file path:%s\n",file_path);
   create_dir(file_path);
   recv_file(sockfd,temp);
   }
}


int recv_ser_dir_file(int sockfd,char *to_dir_name,int size)
{
  char message_buff[512];
  char buff[512];
  
  char temp[512];
  char *file_name=NULL;
  char file_path[1024];
  int  i=0;
  int  dn_size;
  char *temp_p=NULL;
  while(1)
  {
     int i=0;
     memset(file_path,0,1024);
     memset(message_buff,0,512);
     memset(temp,0,512);
     printf("!!!!!hello\n");
     read(sockfd,message_buff,sizeof(message_buff));
     printf("%s-------------------------------\n",message_buff);
     if(strlen(message_buff) < 4)
     break;
     temp_p= message_buff+size;
     memset(buff,0,512);
     while(1)
     {
      if(temp_p[i]!='*')
      { 
      buff[i]=temp_p[i];
      i++;
      }else{
      break;
      }
     }
     strcpy(temp,to_dir_name);
//     printf("%s\n",temp);
     strcat(temp,buff);
     printf("dir:%s\n",temp);
     dn_size = strlen(temp);
     while(1)
     {
     if(temp[dn_size--] == '/')
     {
     file_name=&temp[dn_size+2];
     break;
     }
     }
   printf("%d\n",(dn_size+2));
   snprintf(file_path,(dn_size+2),"%s",temp);
   printf("====%s\n",file_name);
   printf("file path:%s\n",file_path);
   create_dir(file_path);
   recv_file(sockfd,temp);
  }
}
