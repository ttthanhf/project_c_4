#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_USER 100
#define MAX_LETTER 100

char listUser[MAX_USER][3][MAX_LETTER] ; // listUser[STT][full name/user name/password][số kí tự]
int countUser;                           //                    0  /     1   /   2

FILE *file;
char fileTmp[] = "Data.txt";
char fileTmp2[] = "Log.txt";
char tmpAddress[256];


void creatFolder()
{
    char name [10];
    char cmmnd[]={"md "};
    char address[256];
    getcwd(address, 256); // hàm lấy path hiện tại
    int status;

    strcat(cmmnd, listUser[countUser][1]);
    status=system (cmmnd); // hàm tạo folder
    strcat(address, "\\");
    strcat(address, listUser[countUser][1]);
    int ch = chdir(address);
    printf("%s", address);


    strcat(tmpAddress, listUser[countUser][1]) ;
    strcat(tmpAddress, fileTmp);
    file = fopen(tmpAddress,"w");
    fclose(file);

    int len = strlen(tmpAddress);
    for(int i = 0; i <= len; i++)
        tmpAddress[i] = '\0';

    strcat(tmpAddress, listUser[countUser][1]) ;
    strcat(tmpAddress, fileTmp2);
    file = fopen(tmpAddress,"w");
        //fprintf(file,"da ghi");
    fclose(file);


}
int loginPage() // ham in ra cac lua chon
{
    int choice ;
    printf("\n1 - Sign up ");
    printf("\n2 - Login ");
    printf("\nChoose : ") ;
    scanf("%d", &choice ) ;
    return choice ;
}
int calendarPage()
{
    int choice ;
    printf("\nlich thieu nang") ;
    printf("\n1. Add note");
    printf("\n2. Delete ");
    printf("\n3. Logout ");
    printf("\nChoose : ");
    scanf("%d", &choice ) ;
    return choice ;

}


int checkUserName( char userNameTmp[])
{
    char userNameFile[MAX_LETTER];
    char line[256];
    file = fopen("acc2.txt","r");

    while (fgets(line, sizeof(line), file))
    {
        fscanf(file,"Name: %s", &userNameFile);
        if(strcmp(userNameTmp, userNameFile) == 0)
            return 0;
    }

    return 1;

}


int checkPassword(char *passwordTmp)    // mat khau phai co tu 8 ki tu tro len va co chu hoa va chu thuong
{
    int length = strlen(passwordTmp), upperCase = 0, lowerCase = 0, number = 0;
    do
    {
        if(*passwordTmp != '\0')
        {
            if(*passwordTmp >= 65 && *passwordTmp <= 90) // do chu in hoa A(65) -> Z(90)
                upperCase++;
            if(*passwordTmp >= 97 && *passwordTmp <= 122)   // do chu thuong a(97) -> z(122)
                lowerCase++;
            if(*passwordTmp >= 49 && *passwordTmp <= 58)
                number++;
            passwordTmp++;
        }
    }
    while( *passwordTmp != '\0') ; // nếu kí tự != rỗng
    if ((upperCase > 0) && (lowerCase > 0) && (number > 0) && (length >=8))
        return 1;
    return 0;
}


int signUp()
{
    char userNameTmp[MAX_LETTER] ; // cac mang tam
    char retypePassword[MAX_LETTER] ;
    char passwordTmp[MAX_LETTER] ;
    char fullNameTmp[MAX_LETTER] ;

    fflush(stdin);
    printf(" Full name : ");
    scanf("%[^\n]", &fullNameTmp);

    printf(" Username : ");
    do
    {
        scanf("%s", &userNameTmp);
        if(checkUserName(userNameTmp) == 0)
            {
                printf(" Username unavailable.");
                printf(" Please input username again : ");
            }
        else printf(" Username available");
    }
    while(checkUserName(userNameTmp) == 0) ;

    printf("\n");
    printf(" Password : ");
     do
    {
        scanf("%s", &passwordTmp);
        if(checkPassword(passwordTmp) == 0)
            {
                printf(" Wrong format. At least 8 characters with uppercase letters and numbers. ");
                printf(" Please input password again : ");
            }
        else
        {
            printf(" Retype password: ");
            do
            {
                scanf("%s", &retypePassword);
                if( strcasecmp(retypePassword, passwordTmp) == 0)
                printf(" Sign up successfully. \n");
                else
                   {
                        printf(" Wrong password.");
                        printf(" Please retype password again : ");
                   }
            }
            while(strcasecmp(retypePassword, passwordTmp) != 0) ;


        }

    }
    while(checkPassword(passwordTmp) == 0) ;

    strcpy(listUser[countUser][0], fullNameTmp) ; // gan cac fullname, username, password vao mang chinh
    strcpy(listUser[countUser][1], userNameTmp) ;
    strcpy(listUser[countUser][2], passwordTmp) ;

    file = fopen("acc2.txt","a");
        fprintf(file,"Fullname: %s\nName: %s\nPass: %s\n",fullNameTmp, userNameTmp, passwordTmp);
    fclose(file);
    creatFolder();

    countUser++;
}

int checkPasswordLogin( char passwordTmp[])
{
     for(int i = 0; i < countUser; i++)
    {
      if( strcmp( passwordTmp, listUser[i][2]) == 0) // so sánh 2 mảng
        return 1;
    }
    return 0;
}



int login()
{
    char userNameTmp[MAX_LETTER] ;
    char passwordTmp[MAX_LETTER] ;
    char userNameFile[MAX_LETTER] ;
    char passwordFile[MAX_LETTER] ;
    char line[256];
    int check = 0;


    file = fopen("acc2.txt","r");

    printf(" Username : ");
    scanf("%s", &userNameTmp);
    printf(" Password : ");
    scanf("%s", &passwordTmp);
    while (fgets(line, sizeof(line), file))
    {
        fscanf(file,"Name: %s", &userNameFile);
        fscanf(file,"Pass: %s", &passwordFile);

        if(strcmp(userNameTmp, userNameFile) == 0 && strcmp(passwordTmp,passwordFile) == 0)
        {
            check = 1;
        }
    }
    fclose(file);

    if (check == 1) {
        printf("dang nhap thanh cong");
         system("pause");
        return 1;
    }
    else
     {
          while( check == 0)
        {
            printf(" The user name or password that you have entered is incorrect.\n ");
            printf(" Username : ");
            scanf("%s", &userNameTmp);
            printf(" Password : ");
            scanf("%s", &passwordTmp);
            if(strcmp(userNameTmp, userNameFile) == 0 && strcmp(passwordTmp,passwordFile) == 0)
        {
            check = 1;
        }
        }
            printf(" Login successfully. \n") ;
            return 1;
      }
  /*  if(checkUserName(userNameTmp) == 0 && checkPasswordLogin(passwordTmp) == 1) // check cả 2 cái nếu 1 trong 2 sai -> sai
    {
          printf(" Login successfully. \n") ;
          return 1; // return 1 để dùng cho logout
    }
    else
    {
           while(checkUserName(userNameTmp) == 1 || checkPasswordLogin(passwordTmp) == 0)
        {
            printf(" The user name or password that you have entered is incorrect.\n ");
            printf(" Username : ");
            scanf("%s", &userNameTmp);
            printf(" Password : ");
            scanf("%s", &passwordTmp);
        }
            printf(" Login successfully. \n") ;
            return 1;
    } */
}


int main()
{
    int userChoice ;
    int logined = 0 ; // khi logined = 0 -> chưa đăng nhập, còn logined = 1 -> đã đăng nhập
        if(logined == 0)
            do
            {
                userChoice = loginPage(); // goi ham de in ra menu
                switch(userChoice)
                {
                    case 1 : // gọi ham sign uup
                        {
                            signUp() ;
                            system("pause"); // lệnh dừng
                            system("cls"); // lệnh xóa màn hình
                        }
                            break ;
                    case 2 : // goi ham login
                    {
                        logined = login();
                         system("cls"); // xoa man hinh khi login thành công
                         do
                        {
                            userChoice = calendarPage() ; // in ra màn hình giao diện ứng dụng
                            switch(userChoice)
                                {
                                    case 1 :
                                        {
                                            printf("added\n"); // này chưa làm nên để đại
                                            system("pause");
                                            system("cls");
                                        }
                                            break ;
                                    case 2 :
                                        {
                                            printf("deleted\n"); // như trên
                                            system("pause");
                                            system("cls");
                                        }
                                             break;
                                    case 3 :
                                        {
                                            logined = 0 ; // cho logined trở về 0 để quay về vòng lặp hàng 185
                                            system("cls");
                                        }
                                            break ;
                                }
                        }
                        while ( userChoice != 3); // thoát khỏi giao diện lịch khi người dùng bấm đăng xuất
                    }
                    ; break ;

                }
            }
            while ( userChoice < 2 && userChoice > 0 );
      return 0;
}
