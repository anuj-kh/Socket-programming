/* f20171508@hyderabad.bits-pilani.ac.in Anuj Kharbanda */

/* Download index.html and logo via proxy using sockets in C */

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1000 

char* getMessage(char *h,char *h2,char *h3,int flag)
{
    char *mes,*m1,*m2,*m3,*m4;
    m1 = "GET http://";
    if(flag==0)
    {
        m2 = " HTTP/1.1\r\n"
            "Host: ";
    }
    else
    {
        m2 = " HTTP/1.1\r\n"
            "Host: ";
    }
    m3 = "\r\n"
        "Proxy-Authorization: Basic ";
    m4= "\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "\r\n";
    mes=malloc(strlen(m1)+strlen(m2)+strlen(m3)+strlen(m4)+strlen(h)+strlen(h2)+strlen(h3)+1);
    mes[0] = '\0';  
    strcat(mes,m1);
    strcat(mes,h);
    strcat(mes,m2);
    strcat(mes,h2);
    strcat(mes,m3);
    strcat(mes,h3);
    strcat(mes,m4);
    return mes;
}

char* encode(char *id,char* pass)
{
    char *mes,*m1;
    m1=":";
    mes=malloc(strlen(id)+strlen(pass)+strlen(m1)+1);
    mes[0]='\0';
    strcat(mes,id);
    strcat(mes,m1);
    strcat(mes,pass);
    // printf("%s\n",mes);
    unsigned long int len=strlen(mes);

    char encodeChar[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

    char *output=(char*)malloc(SIZE*sizeof(char)); 

    int temp,bitsRead=0,temp2=0,value=0,ctn=0,temp3,k=0; 

    for(int i=0;i<len;i+=3) 
    { 
        bitsRead=0;
        ctn=0;
        value=0;         
        int minn=len-1;
        if(i+2<minn)    
            minn=i+2;
        for (int j=i;j<=minn;j++) 
        { 
            value=value<<8;  
            value=value | mes[j];  
            ctn++;  
        } 
        bitsRead=ctn*8;  
        temp2=bitsRead%3;  
        while(bitsRead!=0)  
        { 
            if(bitsRead>=6) 
            { 
                temp=bitsRead-6; 
                temp=(value>>temp)&63;  
                bitsRead-=6;          
            } 
            else
            { 
                temp=6-bitsRead; 
                temp=(value<<temp)&63;  
                bitsRead=0; 
            } 
            output[k++]=encodeChar[temp]; 
        } 
    }  

    for(int i=1;i<=temp2;i++)  
    { 
        output[k++]='='; 
    } 
  
    output[k]='\0';
    return output;
}

int main(int argc , char *argv[])
{
    char *HOST=argv[1],*HOST2=argv[1],*SERVER=argv[2],*PORT=argv[3],*ID=argv[4],*PASS=argv[5],*FILE1=argv[6],*FILE2=argv[7];

    while(1)
    {
        int socket_desc;
        struct sockaddr_in server;
        char server_reply[1];

        socket_desc=socket(AF_INET,SOCK_STREAM,0);
        if (socket_desc == -1)
        {
            printf("Could not create socket");
        }

        server.sin_addr.s_addr=inet_addr(SERVER);
        server.sin_family=AF_INET;
        server.sin_port=htons(atoi(PORT));

        if(connect(socket_desc,(struct sockaddr *)&server,sizeof(server))<0)
        {
            puts("connect error");
            return 1;
        }
        puts("Connected\n");

        char* encoded=encode(ID,PASS);
        char* message=getMessage(HOST,HOST2,encoded,0);

        printf("%s\n",message);

        if(send(socket_desc,message,strlen(message),0)<0)
        {
            puts("Send failed");
            return 1;
        }
        puts("Data Send\n");

        FILE *fptr;
        fptr = fopen(FILE1,"w");
        if(fptr == NULL)
        {
            printf("Error!");   
            exit(1);             
        }

        int n;
        int count=0,count2=0,loc_count=0;
        char loc[1000];
        do {
            memset(server_reply,0,1);
            n=recv(socket_desc, server_reply , sizeof(server_reply) , 0);
            // printf("%d\n",n);
            
            if(n<0)
            {
                puts("recv failed");
                fclose(fptr);
                return 1;
            }
            else if(n>0)
            {
                // printf("\tcount=%d\tcount2=%d\n",count,count2);
                if(count==-1)
                {
                    // printf("\nDADE\n");
                    fputc(server_reply[0], fptr);
                }
                else
                {
                    printf("%c",server_reply[0]);
                    // printf("\nDAE %d %d\n",count,count2);
                    if(count2!=-1)
                    {
                        if(server_reply[0]=='L' && count2==0)
                            count2++;
                        else if(server_reply[0]=='o' && count2==1)
                            count2++;
                        else if(server_reply[0]=='c' && count2==2)
                            count2++;
                        else if(server_reply[0]=='a' && count2==3)
                            count2++;
                        else if(server_reply[0]=='t' && count2==4)
                            count2++;
                        else if(server_reply[0]=='i' && count2==5)
                            count2++;
                        else if(server_reply[0]=='o' && count2==6)
                            count2++;
                        else if(server_reply[0]=='n' && count2==7)
                            count2++;
                        else if(server_reply[0]==':' && count2==8)
                            count2++;
                        else if(server_reply[0]==' ' && count2==9)
                            count2++;
                        else if(count2==10)
                        {
                            while(server_reply[0]!='\r')
                            {
                                loc[loc_count++]=server_reply[0];
                                // strncat(loc,&server_reply[0],1);
                                memset(server_reply,0,1);
                                n=recv(socket_desc, server_reply , sizeof(server_reply) , 0);
                                // printf("%d\n",n);
                                printf("%c",server_reply[0]);
                            }
                            loc[loc_count++]='\0';
                            count2=-1;
                        }
                        else
                            count2=0;
                    }

                    if(server_reply[0]=='\r' && count==0)
                        count++;
                    else if(server_reply[0]=='\n' && count==1)
                        count++;
                    else if(server_reply[0]=='\r' && count==2)
                        count++;
                    else if(server_reply[0]=='\n' && count==3)
                        count=-1;
                    else 
                        count=0;
                }
            }
        }while (n>0);
        
        puts("Data received\n");
        // puts(server_reply);

        fclose(fptr);
        
        close(socket_desc);

        if(count2==-1)
        {
            char *to=(char*) loc;
            char *token=strtok(to,"//");
            if(strcmp(token,"http:")==0)
            {
                token=strtok(NULL," ");
                if(token[0]=='/')
                    memmove(token, token+1, strlen(token));
                if(token[strlen(token)-1]=='/')
                    token[strlen(token)-1]='\0';
                // printf("%s \n %lu\n",token,strlen(token));
                HOST=token;
                // printf("%s \n %lu\n",HOST,strlen(HOST));
                
                // printf("%s \n %lu\n",HOST2,strlen(HOST2));
            }
            else if(strcmp(token,"https:")==0)
            {
                printf("HTTPS redirect not implemented\n");
                break;
            }
            else
                HOST=(char*) loc;
            // printf("\n%s \n %lu\n",HOST,strlen(HOST));
        }
        else
        {     
            if(strcmp(HOST,"info.in2p3.fr")==0)
            {
                int socket_desc2;
                struct sockaddr_in server2;
                char server_reply2[1];

                socket_desc2 = socket(AF_INET , SOCK_STREAM , 0);
                if (socket_desc2 == -1)
                {
                    printf("Could not create socket");
                }

                server2.sin_addr.s_addr = inet_addr(SERVER);
                server2.sin_family = AF_INET;
                server2.sin_port = htons(atoi(PORT));

                if (connect(socket_desc2 , (struct sockaddr *)&server2 , sizeof(server2)) < 0)
                {
                    puts("connect error");
                    return 1;
                }
                puts("Connected\n");

                // HOST="info.in2p3.fr/cc.gif";
                FILE *fptr3;
                fptr3=fopen(FILE1,"r");
                if(fptr3 == NULL)
                {
                    printf("Error!");   
                    exit(1);             
                }

                int ctt=0;
                char image[1000],imgind=0; 
                do
                {
                    char cc = fgetc(fptr3);
                    if (feof(fptr3))
                        break ;
            
                    // printf("%c", cc);
                    if(ctt!=-1)
                    {
                        if(ctt==0 && (cc=='I' || cc=='i') )
                            ctt++;
                        else if(ctt==1 && (cc=='M' || cc=='m') )
                            ctt++;
                        else if(ctt==2 && (cc=='G' || cc=='g') )
                            ctt++;
                        else if(ctt==3 && cc==' ')
                            ctt++;
                        else if(ctt==4 && (cc=='S' || cc=='s') )
                            ctt++;
                        else if(ctt==5 && (cc=='R' || cc=='r') )
                            ctt++;
                        else if(ctt==6 && (cc=='C' || cc=='c') )
                            ctt++;
                        else if(ctt==7 && cc=='=')
                            ctt++;
                        else if(ctt==8 && cc=='\"')
                            ctt++;
                        else if(ctt==9)
                        {
                            while(cc!='\"')
                            {
                                image[imgind++]=cc;
                                // strncat(loc,&server_reply[0],1);
                                cc = fgetc(fptr3);
                            }
                            image[imgind++]='\0';
                            ctt=-1;
                            break;
                        }
                        else
                        {
                            ctt=0;
                        }
                    }
                }  while(1);
                fclose(fptr3);

                char *tempH;
                char *sl="/";
                tempH=malloc(strlen(HOST)+strlen(image)+strlen(sl)+1);
                tempH[0] = '\0'; 
                strcat(tempH,HOST);
                strcat(tempH,sl);
                strcat(tempH,image);

                printf("%s\n\n",tempH);

                char* message2=getMessage(tempH,HOST2,encoded,1);

                printf("%s\n",message2);

                if( send(socket_desc2 , message2 , strlen(message2) , 0) < 0)
                {
                    puts("Send failed");
                    return 1;
                }
                puts("Data Send\n");

                FILE *fptr2;
                fptr2 = fopen(FILE2,"wb");
                if(fptr2 == NULL)
                {
                    printf("Error!");   
                    exit(1);             
                }

                int n2;
                int countt=0;
                do {
                    memset(server_reply2,0,1);
                    n2=recv(socket_desc2, server_reply2 , sizeof(server_reply2) , 0);
                    // printf("%d\n",n);
                    
                    if(n2<0)
                    {
                        puts("recv failed");
                        fclose(fptr2);
                        return 1;
                    }
                    else if(n2>0)
                    {
                        // printf("\tcount=%d\tcount2=%d\n",count,count2);
                        if(countt==-1)
                        {
                            // printf("\nDADE\n");
                            fputc(server_reply2[0], fptr2);
                        }
                        else
                        {
                            printf("%c",server_reply2[0]);
                            if(server_reply2[0]=='\r' && countt==0)
                                countt++;
                            else if(server_reply2[0]=='\n' && countt==1)
                                countt++;
                            else if(server_reply2[0]=='\r' && countt==2)
                                countt++;
                            else if(server_reply2[0]=='\n' && countt==3)
                                countt=-1;
                            else 
                                countt=0;
                        }
                    }
                }while (n2>0);
                
                puts("Data received\n");
                // puts(server_reply);

                fclose(fptr2);
                
                close(socket_desc2);

            }           
            break;
        }
    }


    return 0;
}