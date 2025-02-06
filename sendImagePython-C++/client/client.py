import os

# filename = input("Ingresa el nombre de la imagen a enviar: ")
filenae = input("Name of the image to send: ")

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect(('127.0.0.1', 8080))

with open(filename, "rb") as image:
	image_size = os.path.getsize(filename)

	socket.sendall(image_size.to_bytes(4, byteorder='big'))

	image_data = image.read(2048)
	while image_data:
		socket.send(image_data)
		image_data = image.read(2048)


# print("Imagen Enviada!")
print("Image sent")
image.close()
socket.close()

