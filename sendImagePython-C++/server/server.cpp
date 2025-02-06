#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[]){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        // perror("Error al crear el socket");
        perror("Error to create socket");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        // perror("Error al crear el socket");
        perror("Error to create socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociate IP and Port to the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        // perror("Error al asociar socket a IP y puerto");
        perror("Error to associate IP and Port to socket");
        exit(EXIT_FAILURE);
    }
    // Put the socket in listening
    if (listen(server_fd, 3) < 0)
    {
        // perror("Error al poner socket en escucha");
        perror("Error puttin socket to listen");
        exit(EXIT_FAILURE);
    }
    // Accept the connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        // perror("Error al aceptar la conexion");
        perror("Error to accept connection");
        exit(EXIT_FAILURE);
    }

    // Receive the size of the image
    uint32_t image_size;
    int n = recv(new_socket, &image_size, sizeof(image_size), 0);
    if (n < 0)
    {
        // perror("Error al recibir el tamaño de la imagen");
        perror("Error to receive image size");
        exit(EXIT_FAILURE);
    }

    // Convert to host byte order
    image_size = ntohl(image_size);
    // printf("Recibiendo imagen de tamao %u bytes \n", image_size);
    printf("Receiving image of size %u bytes \n", image_size);

    // Allocate buffer for the image data
    unsigned char *Rbuffer = (unsigned char *) malloc(image_size);
    if(!Rbuffer){
        // perror("Error al alojar la memoria");
        perror("Error to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Receive the imgae data in chunks
    int bytes_received = 0;
    while (bytes_received < image_size)
    {
        n = recv(new_socket, Rbuffer + bytes_received, image_size - bytes_received, 0);
        if (n < 0)
        {
            // perror("Error la recibir datos de la imagen");
            perror("Error to receive image data");
            exit(EXIT_FAILURE);
        }
        bytes_received += n;
    }

    // Save the image data into a file
    FILE *image = fopen("received-image.jpg", "wb");    // Always save the image as .jpg format
    if(image == NULL){
        // perror("Error al crear la imagen");
        perror("Failed to create image");
        exit(EXIT_FAILURE);
    }
    fwrite(Rbuffer, sizeof(unsigned char), image_size, image);
    fclose(image);

    free(Rbuffer);
    close(new_socket);
    close(server_fd);
    

    return 0;
}
