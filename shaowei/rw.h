#ifndef __RW_H
#define __RW_H
#define MAX_LEN 10240  
#define MAXPATH 512

  struct login{
     char user[128];
     char key[128];
     };

  struct cmd{
     char cmd[16];
     char server_cmd[512];
     char client_cmd[512];
  };

  int  send_file(int sockfd,char *file_name);// over
  int  recv_file(int sockfd,char *file_name);//over
  int  scan_dir(char *dir,int depth);//over
  int  rm_dir(char *dir);//over
  int  print();//over
  int  find_value(char *file_name,char *name,char *find_buff);//over
  int  message(int sockfd,char *buffer,int len);
  int  create_dir(char *path_name);//over
  int  rm_dir_file(char *dir);//over
  int  send_dir_file(int sockfd,char *from_dir_name);//over
  int  recv_dir_file(int sockfd,char *to_dir_name);//over
  int  recv_ser_dir_file(int sockfd,char *to_dir_name,int size);//over
  int  client_login(int sockfd);//over
  char *server_login(int sockfd);//over
  char *compare(char *net_name,char *user_list_path);//over
  int  recv_list_all(int sockfd);//over
  int  list_all(int sockfd,char *file_dir,int size);//over
  int  itoa(long num,char size_buff);
#endif
