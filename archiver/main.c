#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void archKat(FILE *archive,char *path,int num){
    int nf = 1,c;
    char pathf[100];
    struct dirent *read;
    struct stat strd;
    FILE *rd;
    DIR *K = opendir(path);
    while((K == NULL)||(strlen(strrchr(path,'/')) <= 1)){
        printf("Невозможно открыть каталог, введите корректный путь:\n");
        scanf("%s",path);
        K = opendir(path);
    }
    if(num != 0) fprintf(archive,"\n");
    fprintf(archive,"=========> Каталог №%d %s",num,strrchr(path,'/')+1);
    strcat(path,"/");
    strcpy(pathf,path);
    while((read = readdir(K)) != NULL){
        lstat(strcat(pathf,read->d_name),&strd);
        if(S_ISDIR(strd.st_mode)){
            if (strcmp(".", read->d_name) == 0 || strcmp("..", read->d_name) == 0){
                strcpy(pathf,path);
                continue;
            }
            archKat(archive,pathf,num + 1);
        }else{
            rd = fopen(pathf,"r");
            fprintf(archive,"\n%d->%d) Имя файла: %s\nРазмер файла: %ld байт\n\n",num,nf,read->d_name,strd.st_size);
            while((c = fgetc(rd)) != EOF) {
                fputc(c, archive);
            }
            fprintf(archive,"\n");
            fclose(rd);
            remove(pathf);
            nf++;
        }
        strcpy(pathf,path);
    }
    closedir(K);
    rmdir(path);
    fprintf(archive,"<========= Конец каталога №%d",num);
}
 int main() {
    char path[100];
    FILE *archive = fopen("archive","a");
    printf("Введите путь к каталогу, который хотите заархивировать:\n");
    scanf("%s",path);
    archKat(archive,path,0);
    printf("Архивация завершена!\n");
    fclose(archive);
}
