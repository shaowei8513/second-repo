#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#include"rw.h"

#define MAXPATH 512
int  itoa(long num,char size_buff)
{
  int i=0;
  int m;
  char str[1024];
  do{
       if (num < 0)
       printf("please in put right bumber!\n");
       if(num < 10)
       {
         str[i] = num %10 + '0';
         i++;
         str[i] = '\0';
         m = i;
                                      // printf("m=:%d\n",m);
         break;
        } else{
          str[i] = num % 10 + '0';
          num /= 10;
          i++ ;
       }
    }while(num > 0);
                                        //printf("string :%s\n",str);
 
   for(i=0;i < m/2;i++ )
   {
    char temp;
    temp = str[i];
    str[i] = str[m-i-1];
    str[m-i-1]= temp;
   }  
   strcpy(size_buff,str);
 return 0;
}

/************function:in config file,find "="right side valaue ***********/

int find_value(char *file_name,char *name,char *find_buff)
{
    FILE   *fdconfig;
    char   configbuff[1024];
    char   *pstr1;
    char   *pstr2;
    char   *pstr3;
    char   buff3[1024];
    char   buffer[1024];
    
   if((fdconfig=fopen(file_name,"r") )== NULL)
   {
   printf("open wrong");
   exit(0);
   }
   while (!feof(fdconfig))
   {
     memset(buff3,0,1024);
     memset (buffer,0,sizeof(buffer));
     memset(configbuff,0,sizeof(configbuff));
     pstr2 = buffer;
     pstr3 = buff3;
     if(NULL == fgets(configbuff,1024,fdconfig))
     break;
     pstr1 = configbuff;

     while(*pstr1 != '\0')
     {
        if('#' == *pstr1)
        break;
        if((' ' == *pstr1)||('\n' ==*pstr1)||('\t'==*pstr1)) 
        {
        pstr1++;
        }else{  
        *(pstr2++) = *(pstr1++);
        if(*pstr1 == '=')
        {
        *(pstr2++) = '\0';
        *(pstr1++);
        break;
        }
        }
     }
     if(strlen(buffer) <  1)
     continue;
 
     while(*pstr1 != '\0')
     {
     if((' ' == *pstr1)||('\n' == *pstr1)||('\t'== *pstr1)) 
     {
     pstr1++;
     }else{  
     *(pstr3++) = *(pstr1++);
     }
     }
     if((strcmp(buffer,name)==0))
     {
     strcpy(find_buff,buff3);
     } 
  }
  return 0;
}

/************function:creat a dir or many dir ***********/

int create_dir(char *path_name) 
{
   char dir_name[512];
   memset(dir_name,0,512);
   strcpy(dir_name,path_name);
   int i,len = strlen(dir_name);
   if(dir_name[len-1] != '/') 
   strcat(dir_name,"/");
  
   len = strlen(dir_name);
   for(i=1;i<len;i++)
    {
     if(dir_name[i] == '/')
      {
        dir_name[i] = '\0';
       if(access(dir_name,0)!=0)
         {
           if(mkdir(dir_name,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) < 0)
            {
              printf("mkdir error\n");
              exit(0);
            }
         }
        dir_name[i] ='/';  
      }
    }
 }

/************function:delete the dir and the files of the dir ***********/

int  rm_dir(char *dir)          //dir scan func
  {
    DIR     *dp;
    struct  dirent  *entry;
    struct  stat    statbuff;
    char    cwd_path[MAXPATH]; 
    int     fd;
    if(!(dp=opendir(dir)))
    {
    printf("can't open:%s\n",dir);
    exit(0);
    }
    chdir(dir); 
    if (getcwd(cwd_path,sizeof(cwd_path)) == NULL)
    {  
    printf("get dirname failed!\n");
    exit(0);
    }
    printf("chdir:%s\n",cwd_path);
    while((entry=readdir(dp))!=NULL)
    {
    lstat(entry->d_name,&statbuff); 
     if(S_IFDIR&statbuff.st_mode) 
     {
     if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name)== 0)
     continue;
     printf("#####:%s\n",entry->d_name);
     rm_dir(entry->d_name);  
     } 
     }
    chdir("..");                //back up
    if((fd=remove(dir)) < 0)
    {
    printf("falied\n");
    exit(0);
    }
    closedir(dp);
}

/************function: integer change to char string ***********/

int rm_dir_file(char *dir)          //dir scan func
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
     rm_dir_file(entry->d_name);  
     }else{
     char path_buff[1024];
     getcwd(path_buff, 1024);
     printf("%s\n",path_buff);
     strcat(path_buff,"/");
     strcat(path_buff,entry->d_name);
     printf("aa:%s\n",path_buff);
     remove(path_buff);
     }
   }
   chdir("..");                //back up
   remove(dir);
   closedir(dp);
}
/************function: integer change to char string ***********/

char *compare(char *net_name,char *user_list_path)
{
    FILE  *fd;
    char  buff[512];
    char  buf[512];
    char  *temp=NULL;
    char  *tp=NULL;
    temp =(char *)malloc(512);
    fd=fopen(user_list_path,"r");

   while(!feof(fd))
   { 
   int i=0;
   memset(buff,0,512);
   if(NULL== fgets(buff,512,fd))
   break;
   memset(buf,0,512);
   memset(temp,0,512);
   snprintf(buf,strlen(buff),"%s",buff);
   printf(":::%s\n",buf);
   memcpy(temp,buf+4,strlen(buff)-4);
   printf("temp::%s\n",temp);
   if(strcmp(net_name,temp)==0)
   {
   printf("we need:%s\n",temp);
   tp=temp;
   break;
   }else{
   continue;
   } 
   } 
   fclose(fd);
   return tp;
}

