import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QTextEdit, QVBoxLayout, QWidget
from PySide6.QtBluetooth import QBluetoothDeviceDiscoveryAgent, QBluetoothSocket

class BluetoothApp(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Bluetooth RFCOMM 통신 예제")
        self.setGeometry(100, 100, 400, 300)

        self.central_widget = QWidget(self)
        self.setCentralWidget(self.central_widget)

        self.layout = QVBoxLayout()
        self.central_widget.setLayout(self.layout)

        self.text_edit = QTextEdit(self)
        self.layout.addWidget(self.text_edit)

        self.send_button = QPushButton("데이터 송신", self)
        self.send_button.clicked.connect(self.send_data)
        self.layout.addWidget(self.send_button)

        self.receive_button = QPushButton("데이터 수신", self)
        self.receive_button.clicked.connect(self.receive_data)
        self.layout.addWidget(self.receive_button)

        self.bluetooth_socket = None

        self.discovery_agent = QBluetoothDeviceDiscoveryAgent()
        self.discovery_agent.deviceDiscovered.connect(self.device_discovered)

    def device_discovered(self, info):
        # Bluetooth 장치를 발견할 때 호출됩니다.
        self.text_edit.append(f"Bluetooth 장치 발견: {info.name()} [{info.address().toString()}]")
        self.connect_to_bluetooth_device(info.address())

    def connect_to_bluetooth_device(self, device_address):
        if self.bluetooth_socket is not None:
            self.bluetooth_socket.close()

        self.bluetooth_socket = QBluetoothSocket()
        self.bluetooth_socket.connectToService(device_address, 1)  # RFCOMM 채널 1에 연결

    def send_data(self):
        if self.bluetooth_socket is None:
            self.text_edit.append("Bluetooth 장치에 연결되지 않았습니다.")
            return

        data_to_send = "Hello, Bluetooth!"
        self.bluetooth_socket.write(data_to_send.encode('utf-8'))
        self.text_edit.append(f"데이터 송신: {data_to_send}")

    def receive_data(self):
        if self.bluetooth_socket is None:
            self.text_edit.append("Bluetooth 장치에 연결되지 않았습니다.")
            return

        received_data = self.bluetooth_socket.readAll()
        self.text_edit.append(f"데이터 수신: {received_data.data().decode('utf-8')}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = BluetoothApp()
    window.show()

    # Bluetooth 장치 검색 시작
    window.discovery_agent.start()

    sys.exit(app.exec())



# import sys
# from PySide6.QtCore import Qt
# from PySide6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QPushButton, QTextEdit, QListWidget, QWidget
# from PySide6.QtBluetooth import QBluetoothDeviceDiscoveryAgent,QBluetoothServiceDiscoveryAgent, QBluetoothAddress, QBluetoothServiceInfo, QBluetoothSocket

# class BluetoothChatApp(QMainWindow):
#     def __init__(self):
#         super().__init__()

#         self.setWindowTitle("Bluetooth Chat")
#         self.setGeometry(100, 100, 400, 400)

#         self.layout = QVBoxLayout()

#         self.device_list = QListWidget(self)
#         self.layout.addWidget(self.device_list)

#         self.text_edit = QTextEdit(self)
#         self.layout.addWidget(self.text_edit)

#         self.send_button = QPushButton("Send", self)
#         self.send_button.clicked.connect(self.send_message)
#         self.layout.addWidget(self.send_button)

#         # Connect 버튼 추가
#         self.connect_button = QPushButton("Connect", self)
#         self.connect_button.clicked.connect(self.connect_to_device)
#         self.layout.addWidget(self.connect_button)

#         self.central_widget = QWidget(self)
#         self.setCentralWidget(self.central_widget)
#         self.central_widget.setLayout(self.layout)

#         self.device_discovery = QBluetoothDeviceDiscoveryAgent()
#         self.device_discovery.deviceDiscovered.connect(self.device_discovered)
#         self.device_discovery.finished.connect(self.scan_finished)
#         self.device_discovery.start()

#         self.selected_devices = []  # 선택한 Bluetooth 장치를 저장할 리스트
#         self.sockets = []  # Bluetooth 소켓을 저장할 리스트

#     def device_discovered(self, info):
#         # Bluetooth 장치가 발견되면 장치 목록에 추가
#         self.device_list.addItem(info.name())

#     def scan_finished(self):
#         # 스캔이 완료되면 스캔 중지
#         self.device_discovery.stop()

#     def connect_to_device(self):
#         selected_items = self.device_list.selectedItems()
        
#         # 선택한 장치를 self.selected_devices 리스트에 추가
#         self.selected_devices.append(self.device_discovery.discoveredDevices()[self.device_list.row(selected_items[0])])
#         #= [self.device_discovery.discoveredDevices()[self.device_list.row(item)] for item in selected_items]

#         if len(self.selected_devices) == 2:
#             # 두 개의 Bluetooth 소켓 생성 및 설정 (수정된 부분)
#             self.sockets = [QBluetoothSocket(QBluetoothServiceInfo.L2capProtocol),
#                             QBluetoothSocket(QBluetoothServiceInfo.L2capProtocol)]


#             for i, socket in enumerate(self.sockets):
#                 socket.connected.connect(lambda i=i: self.on_connected(i))
#                 socket.disconnected.connect(lambda i=i: self.on_disconnected(i))
#                 socket.readyRead.connect(lambda i=i: self.on_ready_read(i))

#                 # 선택한 장치에 연결
#                 socket.connectToService(QBluetoothAddress(self.selected_devices[i].address()), QBluetoothServiceInfo.RfcommProtocol)

#             # 선택한 장치 정보를 출력
#             self.text_edit.append(f"Selected devices: {self.selected_devices[0].name()}, {self.selected_devices[1].name()}")
#             self.selected_devices.clear()

#     def on_connected(self, index):
#         # Bluetooth 장치와 연결이 성공하면 메시지 출력
#         self.text_edit.append(f"Connected to Device {index + 1}")

#     def on_disconnected(self, index):
#         # Bluetooth 장치와 연결이 끊기면 메시지 출력
#         self.text_edit.append(f"Disconnected from Device {index + 1}")

#     def on_ready_read(self, index):
#         # Bluetooth 장치로부터 데이터를 수신하면 메시지 출력
#         data = self.sockets[index].readAll()
#         message = str(data, 'utf-8')
#         self.text_edit.append(f"Received from Device {index + 1}: {message}")

#     def send_message(self):
#         # 사용자가 "Send" 버튼을 클릭하면 메시지 전송
#         message = self.text_edit.toPlainText()
#         if message:
#             for socket in self.sockets:
#                 socket.write(message.encode('utf-8'))
#             self.text_edit.clear()

#     def closeEvent(self, event):
#         # 애플리케이션 종료 시 Bluetooth 소켓 닫기
#         for socket in self.sockets:
#             socket.close()
#         event.accept()

# if __name__ == "__main__":
#     app = QApplication(sys.argv)
#     window = BluetoothChatApp()
#     window.show()
#     sys.exit(app.exec())
