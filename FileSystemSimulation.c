#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100

struct fileNode{
    char fileName[30];
    char fileContent[100];
    int id;
    struct fileNode* fChild;
    struct fileNode* parent;
    struct fileNode* nSibling;
};

char location[MAX] = "";

struct fileNode* createRootfile(char fileName[],int size){

    struct fileNode *root = (struct fileNode*)malloc(sizeof(struct fileNode));
    strcpy(root->fileName,fileName);
    root->id = 1;
    root->parent = NULL;
    root->fChild = NULL;
    root->nSibling = NULL;

    return root;
}


struct fileNode* find_parent(struct fileNode* parentFolder,char node[]){

    struct fileNode** queue = (struct fileNode**)malloc(sizeof(struct fileNode*)*100);
    int front = 0;
    int rare = 0;

    struct fileNode* prev = parentFolder;

    queue[rare++] = parentFolder->fChild;

    while(front < rare){

        struct fileNode* curr = queue[front++];
        if(!strcmp(curr->fileName,node)){
            return prev;
        }
        prev = curr;

         if(curr->fChild){
             queue[rare++] = curr->fChild;
         }

         if(curr->nSibling){
             struct fileNode* temp = curr->nSibling;
             while(temp != NULL){

                 if(!strcmp(temp->fileName,node)){
                    return prev;
                }
                prev = temp;
                 if(temp->fChild){
                     queue[rare++] = temp->fChild;
                 }
                 temp = temp->nSibling;
             }
         }
    }
}


 struct fileNode* find(struct fileNode* root,char filename[]){

    struct fileNode** queue = (struct fileNode**)malloc(sizeof(struct fileNode*)*100);
    int front = 0;
    int rare = 0;

    queue[rare++] = root;

    while(front < rare){

        struct fileNode* curr = queue[front++];

        if(!strcmp(curr->fileName,filename)){
            return curr;
        }

         if(curr->fChild){
             queue[rare++] = curr->fChild;
         }

         if(curr->nSibling){
             struct fileNode* temp = curr->nSibling;
             while(temp != NULL){

                 if(!strcmp(temp->fileName,filename)){
                    return temp;
                }
                 if(temp->fChild){
                     queue[rare++] = temp->fChild;
                 }
                 temp = temp->nSibling;
             }
         }
    }
    return NULL;
}


struct fileNode* createFile(char fileName[],char fileContent[],int id,int size){

    struct fileNode* newFile = (struct fileNode*)malloc(sizeof(struct fileNode));

    strcpy(newFile->fileName,fileName);
    strcpy(newFile->fileContent,fileContent);
    newFile->id = id;
    newFile->fChild = NULL;
    newFile->nSibling = NULL;

    return newFile;
}

void insertFile(struct fileNode* root,char parent[] , char fileName[],char fileContent[],int id){

    //find parent node
    struct fileNode* parentFile = find(root,parent);

    //create new file
    int size = strlen(fileName);
    struct fileNode* newnode = createFile(fileName,fileContent,id,size);
    newnode->parent = parentFile;

    //case 1 parentfile -> child(exist)
    if(parentFile->fChild){
        struct fileNode* firstChild = parentFile->fChild;
        struct fileNode* temp = firstChild;
        while(temp->nSibling != NULL){
            temp = temp->nSibling;
        }
        temp->nSibling = newnode;
    }
    //case 2 parentfile !-> child(not exist)
    else if(!(parentFile->fChild)){
        parentFile->fChild = newnode;
    }
}

void deleteFile(struct fileNode* rootfile,char parentFolderName[] , char fileName[]){


    struct fileNode* parentFolder = find(rootfile,parentFolderName);
    struct fileNode* parent = find_parent(parentFolder,fileName);


    if((parent->nSibling != NULL) && !strcmp(parent->nSibling->fileName,fileName)){

        struct fileNode* delnode = parent->nSibling;

        //last sibling
        if(delnode->nSibling == NULL){
            parent->nSibling = NULL;
            free(delnode);
        }
        else{
             parent->nSibling = delnode->nSibling;
             free(delnode);
        }
    }
    else if((parent->fChild != NULL)   &&  !strcmp(parent->fChild->fileName,fileName)){

        struct fileNode* delnode = parent->fChild;

         if(delnode->nSibling == NULL){
            parent->fChild = NULL;
            free(delnode);
        }
        else if(delnode->nSibling != NULL){
             parent->fChild = delnode->nSibling;
             free(delnode);
        }
    }

}


void Rename(struct fileNode* rootfile,char fileName[],char newFileName[]){

    struct fileNode* file = find(rootfile,fileName);
    strcpy(file->fileName,newFileName);

}


void view(struct fileNode* root){

    struct fileNode* mainfolders = root->fChild;

    while(mainfolders != NULL){
        if(mainfolders->id == 1){
            printf("/%s\n",mainfolders->fileName);
        }else{
        printf("%s\n",mainfolders->fileName);
        }
        mainfolders = mainfolders->nSibling;
    }
}

void manhelp(){
    printf("\ncommands");
    printf("\n\nman -> info of all commands");
    printf("\n\ncd -> go to file/folder");
    printf("\n\nls -> list all file/folder of current directory");
    printf("\n\nmkdir -> create folder");
    printf("\n\ntouch -> create file");
    printf("\n\nrm -> delete file/folder");
    printf("\n\ncp -> copy file/folder");
    printf("\n\ncat -> display file content");
    printf("\n\nrename -> rename file");
    printf("\n\nmv -> move file/folder");
    printf("\n\npwd -> file path of present working directory");
    printf("\n\nfind -> find file exist or not");
    printf("\n\nexit -> exit from system\n\n");
}

int main(){

    char rootfilename[]= "c:";

    struct fileNode* rootfile = createRootfile(rootfilename,4);

    int entrycode;
    char fileName[30];
    char parentFileName[100][100];
    char content[100];
    char fileRename[30];
    int fileType;
    char temp[10];          //temp for gets it read next line of scanf

    int top =-1;
    strcpy(parentFileName[++top] , "c:");

    char instruction[100];

    char base[100] = "United2004@DESKTOP-Ubuntu:~$";


    while(1){

        printf("\n%s ",base);
        gets(instruction);

        char* token = strtok(instruction," ");

        if(!strcmp(token,"cd")){

            token = strtok(NULL," ");

            if(!strcmp(token,"..")){
                int len = strlen(base);
                int lendir = strlen(parentFileName[top]);
                lendir+=2;
                top--;
                base[len-lendir] = '\0';
                strncat(base,"$",1);
            }
            else if(!strcmp(token,"~")){
                strcpy(base,"United2004@DESKTOP-Ubuntu:~$");
                top = 0;
            }
            else{
                if(find(rootfile,token)){
                    strcpy(parentFileName[++top],token);
                    int len = strlen(base);
                    base[len-1] = '\0';
                    strncat(base,"/",1);
                    strcat(base,token);
                    strncat(base,"$",1);
                }else{
                    printf("'%s' file not exist",token);
                }
            }
        }
        else if(!strcmp(token,"ls")){
            view(find(rootfile,parentFileName[top]));
        }
        else if(!strcmp(token,"touch")){
            token = strtok(NULL," ");
            char contentfile[] = "filegenerated";
            insertFile(rootfile,parentFileName[top],token,contentfile,0);
        }
        else if(!strcmp(token,"mkdir")){
            token = strtok(NULL," ");
            char contentfile[] = "filegenerated";
            insertFile(rootfile,parentFileName[top],token,contentfile,1);
        }
        else if(!strcmp(token,"rm")){
            token = strtok(NULL," ");
            if(find(rootfile,token)){
                deleteFile(rootfile,parentFileName[top],token);
            }else{
                printf("'%s' file not exist",token);
            }
        }
        else if(!strcmp(token,"find")){
            strcpy(location,"");
            token = strtok(NULL," ");
            if(find(rootfile,token)){
                char nodefilename[30];
                strcpy(nodefilename,"");
                struct fileNode* file = find(rootfile,token);
                while(file!=NULL){
                    strcpy(nodefilename,file->fileName);
                    strcat(nodefilename,"/");
                    strcat(nodefilename,location);
                    strcpy(location,nodefilename);
                    strcpy(nodefilename,"");
                    file = file->parent;
                }
                printf("%s",location);
            }
            else{
                printf("'%s' file not exist",token);
            }
        }
        else if(!strcmp(token,"pwd")){
            strcpy(location,"");
            for(int i=0;i<=top;i++){
                strcat(location,parentFileName[i]);
                strncat(location,"/",1);
            }
            printf("%s",location);
        }
        else if(!strcmp(token,"cat")){
            token = strtok(NULL," ");
                char str[100];
            if(!strcmp(token,">"))
               {
                   token = strtok(NULL," ");
                   struct fileNode* file = find(rootfile,token);
                    gets(str);
                    strcpy(file->fileContent,str);
               }
            else if(find(rootfile,token)){
                struct fileNode* file = find(rootfile,token);
                printf("%s",file->fileContent);
            }
            else{
                printf("\n'%s' file not exist\n",token);
            }
        }
        else if(!strcmp(token,"mv")){
            char* source = strtok(NULL," ");
            char* destination = strtok(NULL," ");
            if(find(rootfile,source) && find(rootfile,destination)){
                struct fileNode* file = find(rootfile,source);
                insertFile(rootfile,destination,source,file->fileContent,file->id);
                deleteFile(rootfile,parentFileName[top],source);
            }else{
                printf("\nfile not exist\n");
            }
        }
        else if(!strcmp(token,"cp")){
            char* source = strtok(NULL," ");
            char* destination = strtok(NULL," ");
            if(find(rootfile,source)){
                struct fileNode* file = find(rootfile,source);
                insertFile(rootfile,parentFileName[top],destination,file->fileContent,file->id);
            }else{
                printf("\n'%s' file not exist\n",source);
            }
        }
        else if(!strcmp(token,"rename")){
            char* old = strtok(NULL," ");
            char* new = strtok(NULL," ");
            if(find(rootfile,old)){
                struct fileNode* file = find(rootfile,old);
                strcpy(file->fileName,"");
                strcpy(file->fileName,new);
            }else{
                printf("\n'%s' file not exist\n",old);
            }
        }
        else if(!strcmp(token,"man")){
            char* type = strtok(NULL," ");
            if(!strcmp(type,"help")){
                manhelp();
            }else{
                printf("\n%s :commond not found\n",type);
            }
        }
        else if(!strcmp(token,"exit")){
            exit(0);
        }
        else{
            printf("\n%s :commond not found\n",token);
        }
   }
    return 0;
}
