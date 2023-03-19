#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
int isFirst = 0;
void dearchK(FILE *archive,char *buf,char *pathR){
    FILE *wr;
    int i,size,c;
    char *ptr;
    char mbuf[100];
    char ch[2];
    char pathbuf[100];
    strcpy(mbuf,"");
    strcpy(buf,"");
    strcpy(pathbuf,pathR);
    while(1){
        fgets(buf,200,archive);
        if(buf[0] == '\n'){
            strcpy(buf,"");
            continue;
        }else if(buf[0] == '='){
            ch[0] = isFirst + '0';
            for(ptr = strchr(buf,ch[0])+2; ptr < strrchr(buf,'\n'); ptr++)
                strncat(mbuf,ptr,1);
            strcat(pathbuf,"/");
            strcat(pathbuf,mbuf);
            mkdir(pathbuf,0777);
            isFirst++;
            dearchK(archive,buf,pathbuf);
            isFirst--;
            if(isFirst == 0)break;
            strcpy(pathbuf,pathR);
        }else if(buf[0] == '<')break;
        else{
            for(ptr = strchr(buf,':') + 2; ptr < strrchr(buf,'\n'); ptr++)
                strncat(mbuf,ptr,1);
            strcat(pathbuf,"/");
            wr = fopen(strcat(pathbuf,mbuf),"a");
            strcpy(pathbuf,pathR);
            strcpy(buf,"");
            strcpy(mbuf,"");
            fgets(buf,200,archive);
            for(ptr = strchr(buf,':') + 2; ptr < strrchr(buf,' '); ptr++)
                strncat(mbuf,ptr,1);
            size = atoi(mbuf);
            fgets(buf,200,archive);
            strcpy(buf,"");
            for(i = 0; i < size; i++){
                c = fgetc(archive);
                fputc(c,wr);
            }
            fclose(wr);
            printf("%s",buf);
            fgets(buf,200,archive);
        }
        strcpy(buf,"");
        strcpy(mbuf,"");
    }
}
 int main() {
    DIR *K;
    char buf[200];
    char path[100];
    char pathR[100];
    FILE *archive;
    printf("Введите путь к архиву, который хотите разархивировать:\n");
    do{
        scanf("%s",path);
        archive = fopen(path,"r");
        if((archive == NULL)||(strlen(strrchr(path,'/')) <= 1))
            printf("Невозможно открыть архив, введите корректный путь:\n");
    }while((archive == NULL)||(strlen(strrchr(path,'/')) <= 1));
    printf("Введите путь к каталогу,в который хотите извлечь содержимое архива:\n");
    do{
        scanf("%s",pathR);
        K = opendir(pathR);
        if((K == NULL)||(strlen(strrchr(pathR,'/')) <= 1))
            printf("Невозможно открыть каталог, введите корректный путь:\n");
    }while((K == NULL)||(strlen(strrchr(pathR,'/')) <= 1));
    closedir(K);
    dearchK(archive,buf,pathR);
    printf("Разархивация завершена!\n");
    fclose(archive);
    remove(path);
}
