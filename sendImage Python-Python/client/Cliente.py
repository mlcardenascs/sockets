import socket

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect(('25.4.21.248', 8080))

imagen = open('Hellsing6.jpg', 'rb')
imagen_data = imagen.read(2048)

while imagen_data:
    socket.send(imagen_data)
    imagen_data = imagen.read(2048)
    
print('Imagen Enviada!')    
imagen.close()
socket.close()