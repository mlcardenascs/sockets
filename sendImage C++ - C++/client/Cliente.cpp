#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define PORT 8080

int main(int argc, char const *argv[]){
    int sock=0, valread;
    struct sockaddr_in serv_addr;
    //char *hello = "Mensaje de texto al cliente";
    char buffer[1024]={0};

    //Crear socket
    if((sock=socket(AF_INET, SOCK_STREAM,0))<0){
        printf("\n Error al crear el socket\n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){    //Convierte direccion IP de texto a binario (Asi lo requiere C++)
        printf("\n Direccion IP no valida\n");
        return -1;
    }
    if(connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){    //Conectarse a asociar IP y puerto para enviar informacion
        printf("\nFallo la conexion\n");
        return -1;
    }


    int n = 0;
    int siz = 0;
    FILE *picture;
    char buf[50];
    //char *s="";

    picture = fopen("hellsing4.jpg", "rb");
    fseek(picture, 0, SEEK_END);
    siz = ftell(picture);

    sprintf(buf, "%d", siz);
    if((n = send(sock, buf, sizeof(buf), 0)) < 0)
    {
            perror("send_size()");
            return -1;
    }

    char Sbuf[siz];
    fseek(picture, 0, SEEK_END);
    siz = ftell(picture);
    fseek(picture, 0, SEEK_SET); //Going to the beginning of the file

    while(!feof(picture)){
    n = fread(Sbuf, sizeof(char), siz, picture);
    if (n > 0) { /* only send what has been read */
        if((n = send(sock, Sbuf, siz, 0)) < 0) /* or (better?) send(sock, Sbuf, n, 0) */
        {
            perror("send_data()");
            return -1;
            }
        }
    }
    return 0;
}
