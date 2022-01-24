#include <stdio.h>
#include <string.h>



void login_show() {
    char fullname[100], username[100], password[100], retypePassword[100];
    char inName[100], inPass[100], temp1[1000];
    int  check = 0;
    char name[100];
    char pass[100];
    char line[256];

    FILE *file;
    file = fopen("acc2.txt","r");

    printf("nhập username: ");
    scanf("%s",&username);
    printf("nhập password: ");
    scanf("%s",&password);

    while (fgets(line, sizeof(line), file)) {
        fscanf(file,"Name: %s", name);
        fscanf(file,"Pass: %s", pass);

        if(strcmp(username,name) == 0 && strcmp(password,pass) == 0) {
            check += 1;
        }
    }

    if (check == 2) {
        printf("dang nhap thanh cong");
    }
    else {
        printf("dang nhap that bai");
    }
}

void register_show() {
    char fullname[100], username[100], password[100], retypePassword[100];
    char temp;
    
    FILE *file;
    file = fopen("acc2.txt","a");

    printf("nhập fullname: ");
    scanf("%c",&temp);
    scanf("%[^\n]",&fullname);
    printf("nhập username: ");
    scanf("%s",&username);
    printf("nhập password: ");
    scanf("%s",&password);
    printf("nhập lại password: ");
    scanf("%s",&retypePassword);

    if(strcmp(password, retypePassword) == 0) {
        fprintf(file,"Fullname: %s\nName: %s\nPass: %s\n",fullname, username, password);
        printf("dang ky thanh cong");
        fclose(file);
    }
    
    else {
        printf("sai roi");
    }
}

void choose() {
    int choose;
    printf("1. login - 2. register");
    scanf("%d",&choose);

    switch (choose)
    {
    case 1:
        login_show();
        break;
    case 2:
        register_show();
        break;
    }
}

int main() {

    choose();
    return 0;
}
