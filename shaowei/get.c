#include<stdio.h>
#include<string.h>

int cmd(char *cmd,char *server_pf,char *client_pf)
 {
	char a[512];
 	char b1[16];
	char b2[512];
	char b3[512];
	int i,j,word=0;
 	
	fgets(a,10,stdin);
	printf("%s\n",a);
 	i=0;j=0;
	for(i;i<strlen(a);i++)
	{
 		if((a[i])!=' ')
		{
		b1[j]=a[i];
		i++;
		j++;
		}
	}
	return 0;
}

int main()
{
	struct cmd{
	char cmd[16];
	char server_cmd[512];
	char client_cmd[512];
	};
	return 0;
}
