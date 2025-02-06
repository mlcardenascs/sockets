#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 8080

int main(int argc, char const *argv[]){
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt=1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	//Crear socket
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0))==0){
		perror("Error al crear el socket");
		exit(EXIT_FAILURE);
	}
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("Error al crear el socket");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){	//Asociar IP y Puerto
		perror("Error al asociar socket a IP y puerto");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd, 3)<0){
		perror("Error al poner socket en escucha");
		exit(EXIT_FAILURE);
	}
	if((new_socket=accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0){
		perror("Error al aceptar la conexion");
		exit(EXIT_FAILURE);
	}

     int n = 0;
    char buf[50];
    unsigned int siz = 65536;
    if ((n = recv(new_socket, buf, sizeof(buf), 0) <0)){
        perror("recv_size()");
        return -1;
    }

    unsigned char Rbuffer[65536];
    n = 0;
    if ((n = recv(new_socket, Rbuffer, siz, 0)) < 0){
    perror("recv_size()");
    return -1;
}

FILE *image;
image = fopen("image.jpg", "wb");
fwrite(Rbuffer, sizeof(char), siz, image);
fclose(image);

printf("Imagen del cliente recibida!");
	return 0;
}
