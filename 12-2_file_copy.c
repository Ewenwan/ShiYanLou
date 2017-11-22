
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
 FILE *fp_src,*fp_tar;

 if(((fp_src=fopen(argv[1],"rb"))==NULL)) 
   {
      printf("error\n");    
      exit(0);
   }
//printf("%s\n",argv[1]);
//printf("%s\n",argv[2]);
fp_tar=fopen(argv[2],"wb");
int ch;
while((ch=fgetc(fp_src))!=EOF)
    fputc(ch,fp_tar);

fcloseall();
//fclose(fp_src);
//fclose(fp_tar);
return 0;

}
