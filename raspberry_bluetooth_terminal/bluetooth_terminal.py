import bluetooth

target_address = "ESP32_MAC_ADDRESS"  # ESP32의 MAC 주소 입력
port = 1  # ESP32와 통신하는 포트 번호 (일반적으로 1)

server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
server_sock.bind(("", port))
server_sock.listen(1)

print("Listening for incoming connections...")
client_sock, client_info = server_sock.accept()
print("Accepted connection from", client_info)

try:
    while True:
        data = client_sock.recv(1024)
        if len(data) == 0:
            break
        print("Received:", data.decode())
except KeyboardInterrupt:
    pass

print("Disconnected.")
client_sock.close()
server_sock.close()
