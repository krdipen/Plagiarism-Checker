#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<dirent.h>
#include<math.h>

typedef struct Pair{
    char key[100];
    int value;
}Pair;

unsigned long hash(char *str){
    unsigned long hash=5381;
    int c;
    while((c=*str++)){
        hash=((hash<<5)+hash)+c;
    }
    return hash%10000;
}

unsigned long prob(char *str){
    unsigned long hash=0;
    int c;
    while((c=*str++)){
        hash=c+(hash<<6)+(hash<<16)-hash;
    }
    return hash%(10000-1)+1;
}

void initialize(Pair* pair){
    for(int i=0;i<10000;i++){
        strcpy(pair[i].key,"NA");
        pair[i].value=0;
    }
}

void initialize_wrt(Pair* pair1,Pair* pair2){
    for(int i=0;i<10000;i++){
        memcpy(pair1[i].key,pair2[i].key,strlen(pair2[i].key)+1);
        pair1[i].value=0;
    }
}

bool token(char ch){
    char token_ch[35] = {
        ' ', '\t', '\n', '.', ',',
        '\'', '(', ')', '\"', '-',
        ';', '?', '!', '<', '>',
        ':', '/', '[', ']', '{',
        '}', '\\', '|', '`', '~',
        '@', '#', '$', '%', '^',
        '&', '*', '=', '+', '_'
    };
    for(int i=0;i<35;i++){
        if(ch==token_ch[i]){
            return true;
        }
    }
    return false;
}

void getmap(char* file_path,Pair* pair){
    FILE* file=fopen(file_path,"r");
	char ch;
    char word[100];
    int i=0;
	while((ch=fgetc(file))!=EOF){
        if(token(ch)){
            word[i]='\0';
            i=0;
            while(true){
                int index=(hash(word)+i*prob(word))%10000;
                if(!strcmp("NA",pair[index].key)){
                    memcpy(pair[index].key,word,strlen(word)+1);
                    pair[index].value=1;
                    break;
                }
                if(!strcmp(word,pair[index].key)){
                    pair[index].value++;
                    break;
                }
                i++;
                if(i==10000){
                    printf("Word Limit Exceeded\n");
                    break;
                }
            }
            i=0;
		}
		else{
            word[i]=ch;
            i++;
		}
	}
	fclose(file);
}

int main(int argc,char *argv[]){
    char* testfile_path=argv[1];
    Pair pair[50][10000];
    initialize(pair[0]);
    getmap(testfile_path,pair[0]);
    char* corpus_path=argv[2];
    strncat(corpus_path,"/",1);
    DIR* corpus=opendir(corpus_path);
    struct dirent* corpusfile_name;
    int i=1;
    char file_name[50][100];
    while((corpusfile_name=readdir(corpus))){
        char corpusfile_path[100];
        memcpy(corpusfile_path,corpus_path,strlen(corpus_path)+1);
        strncat(corpusfile_path,corpusfile_name->d_name,strlen(corpusfile_name->d_name));
        if (!strcmp (corpusfile_name->d_name,".")){
            continue;
        }
        if (!strcmp (corpusfile_name->d_name,"..")){
            continue;
        }
        if (!strcmp (corpusfile_name->d_name,".DS_Store")){
            continue;
        }
        initialize_wrt(pair[i],pair[i-1]);
        getmap(corpusfile_path,pair[i]);
        memcpy(file_name[i],corpusfile_name->d_name,strlen(corpusfile_name->d_name)+1);
        i++;
    }
    for(int j=0;j<10000;j++){
        memcpy(pair[i][j].key,pair[i-1][j].key,strlen(pair[i-1][j].key)+1);
        int count=0;
        for(int k=1;k<i;k++){
            if(pair[k][j].value>0){
                count++;
            }
        }
        if(count>0){
            pair[i][j].value=count;
        }
        else{
            pair[i][j].value=1;
        }
    }
    for(int k=1;k<i;k++){
        long long dot_prod=0;
        long long sq_sum_test=0;
        long long sq_sum_corpus=0;
        for(int j=0;j<10000;j++){
            dot_prod+=pair[0][j].value*pair[k][j].value*pow(log((i-1)/pair[i][j].value),2);
            sq_sum_test+=pow(pair[0][j].value,2)*pow(log((i-1)/pair[i][j].value),2);
            sq_sum_corpus+=pow(pair[k][j].value,2)*pow(log((i-1)/pair[i][j].value),2);
        }
        float cosine=dot_prod/(sqrt(sq_sum_test)*sqrt(sq_sum_corpus));
        printf("%s %0.2f%%\n",file_name[k],cosine*100);
    }
	return 0;
}
