import socket

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.bind(('25.4.21.248', 8080))
socket.listen()

conn, add = socket.accept()

imagen = open('Imagen_Recibida.jpg', 'wb')

imagen_chunk = conn.recv(2048)

while imagen_chunk:
    imagen.write(imagen_chunk)
    imagen_chunk = conn.recv(2048)

print('Imagen Recibida!')
imagen.close()
conn.close()
