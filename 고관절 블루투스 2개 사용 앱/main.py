# """PySide6 port of the bluetooth/btscanner example from Qt v6.x"""

# import sys

# from PySide6.QtCore import Qt
# from PySide6.QtWidgets import QApplication, QWidget

# from device import DeviceDiscoveryDialog


# if __name__ == '__main__':
#     app = QApplication(sys.argv)
#     d = DeviceDiscoveryDialog()
#     d.exec()
#     sys.exit(0)


# import sys

# from PySide6.QtCore import Qt,QCoreApplication
# from PySide6.QtWidgets import QApplication, QWidget,QMainWindow
# from PySide6.QtWidgets import QStackedWidget,QMessageBox,QHBoxLayout,QVBoxLayout

# from viewer import visual_widget
# from pyside_main import Ui_MainWindow


# class MainWindow(QMainWindow):
#     def __init__(self):
#         super().__init__()
#         self.ui = Ui_MainWindow()
#         self.ui.setupUi(self)
#         # 중앙 위젯을 생성하고 설정합니다.
#         central_widget = QWidget()
#         self.setCentralWidget(central_widget)

#         # QStackedWidget을 생성합니다.
#         self.stacked_widget = QStackedWidget()
#         central_layout = QVBoxLayout(central_widget)
#         central_layout.addWidget(self.stacked_widget)

#         # visual_widget 인스턴스 생성
#         widget1 = visual_widget()

#         # QStackedWidget에 위젯 추가
#         self.stacked_widget.addWidget(widget1)

#         # 메인 창에 QStackedWidget을 설정
#         self.setCentralWidget(self.stacked_widget)

# import os
# if __name__ == '__main__':
#     app = QApplication(sys.argv)
    
#     window = MainWindow()
#     window.show()
#     sys.exit(app.exec())


# from PySide6.QtBluetooth import QLowEnergyController, QBluetoothUuid
# from PySide6.QtWidgets import QApplication, QWidget
# import sys

# # 사용할 서비스와 특성 UUID 설정
# SERVICE_UUID = QBluetoothUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9A")
# CHARACTERISTIC_UUID_TX = QBluetoothUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9A")
# CHARACTERISTIC_UUID_RX = QBluetoothUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9A")

# def characteristic_changed(characteristic, value):
#     # 데이터 수신 시 호출되는 함수
#     received_data = value.data()
#     print("Received Data:", received_data)

# def start_communication(controller):
#     # 연결 성공 시 호출되는 함수
#     print("Connected to the device!")

#     # 서비스 검색
#     service = controller.createServiceObject(QBluetoothUuid(SERVICE_UUID))
#     if not service:
#         print("Service not found!")
#         return

#     # 특성 값 변경 이벤트 처리
#     tx_characteristic = service.characteristic(QBluetoothUuid(CHARACTERISTIC_UUID_TX))
#     tx_characteristic.changed.connect(characteristic_changed)

#     # 데이터 송신
#     command = bytearray([0x53, 0x02, 0x64, 0x00, 0xFF, 0x45])

#     data_to_send = command
#     controller.writeCharacteristic(tx_characteristic, data_to_send)

# if __name__ == '__main__':
#     app = QApplication(sys.argv)

#     controller = QLowEnergyController.createCentral()
#     controller.setRemoteAddress("XX:XX:XX:XX:XX:XX")  # BLE 장치 주소 설정
#     controller.connected.connect(start_communication)

#     controller.connectToDevice()

#     sys.exit(app.exec_())



# from PySide6.QtBluetooth import QBluetoothDeviceInfo,QBluetoothAddress,QBluetoothServiceDiscoveryAgent,QBluetoothUuid,QBluetoothDeviceDiscoveryAgent, QLowEnergyController
# from PySide6.QtCore import QObject, Signal
# from PySide6.QtWidgets import QApplication
# import sys
# SERVICE_UUID = QBluetoothUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9A")
# CHARACTERISTIC_UUID_TX = QBluetoothUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9A")
# CHARACTERISTIC_UUID_RX = QBluetoothUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9A")
# class MyBluetoothController(QObject):
#     dataReceived = Signal(bytes)

#     def __init__(self):
#         super().__init__()

#         self.service_uuid = QBluetoothUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9A")
#         self.characteristic_uuid_tx = QBluetoothUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9A")
#         self.characteristic_uuid_rx = QBluetoothUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9A")
#         self.device_address = ''
#         self.disco = QBluetoothDeviceDiscoveryAgent()

#     def device_discovered(self,device_info):
#         print("장치검색")
#         device_name = device_info.name()
#         self.device_address = device_info.address().toString()
#         print(f"Discovered device: {device_name}, Address: {self.device_address}")

#         # 특정 UUID 확인
#         if self.service_uuid in device_info.serviceUuids():
#             print("Device with the desired service UUID found!")
#             print("Device Address:", self.device_address)
#             self.disco.stop()
#             self.connect_to_mac(self.device_address,device_name)
            

#     def connect_uuid(self):
#         print("시작")
#         self.disco.start()
#         self.disco.deviceDiscovered.connect(self.device_discovered)

#     def connect_to_mac(self, mac_address: str, name):
#         addr = QBluetoothAddress(mac_address)
#         device = QBluetoothDeviceInfo(addr,name,1)
#         self.controller = QLowEnergyController.createCentral(device)
#         self.controller.connectToDevice()
#         self.controller.stateChanged.connect(self.on_controller_state_changed)
#         #print(QLowEnergyController.state(self.controller))

#     def on_controller_state_changed(self, new_state):
#         if new_state == QLowEnergyController.ConnectedState:
#             print("Bluetooth 장치와 연결 성공!")
#             self.start_communication()
#         elif new_state == QLowEnergyController.UnconnectedState:
#             print("Bluetooth 장치와 연결이 끊어짐.")
#             #self.controller.disconnectFromDevice()
#             #self.controller.connectToDevice()
#             self.start_communication()
#         else:
#             print("Bluetooth 장치 상태 변경:", new_state)
            

#     def start_communication(self):
#         print("되는거냐")
#         service = self.controller.createServiceObject(QBluetoothUuid(self.service_uuid))
#         if service:
#             print("서비스있대")
#             tx_characteristic = service.characteristic(QBluetoothUuid(self.characteristic_uuid_tx))
#             if tx_characteristic:
#                 print("tx")
#                 tx_characteristic.changed.connect(self.characteristic_changed)
#                 command = bytearray([0x53, 0x02, 0x64, 0x00, 0xFF, 0x45])
#                 self.controller.writeCharacteristic(tx_characteristic, command)
#         else:
#             self.controller.stateChanged.connect(self.on_controller_state_changed)
#             self.controller.connectToDevice()


#     def characteristic_changed(self, characteristic, value):
#         received_data = value.data()
#         self.dataReceived.emit(received_data)

# def main():
#     app = QApplication(sys.argv)

#     controller = MyBluetoothController()
#     controller.connect_uuid()

#     sys.exit(app.exec())

# def handle_received_data(data):
#     print("Received Data:", data)

# if __name__ == '__main__':
#     main()


# import asyncio
# import bleak
# import numpy

# # BLE 주소 (MAC 주소) 및 UUID 설정
# address = '90:38:0C:FE:BD:D2'
# uuid_rx = '6E400002-B5A3-F393-E0A9-E50E24DCCA9A'
# uuid_tx = '6E400003-B5A3-F393-E0A9-E50E24DCCA9A'
# service_uuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9A"

# async def notification_handler(sender, data, callback):
#     spo2_value = numpy.frombuffer(data, dtype=numpy.uint32)
 
#     print("Red values:")
#     print(spo2_value)
 
#     # Update the label text
#     callback(spo2_value)

# async def communicate_with_ble_device():
#     async with bleak.BleakClient(address) as client:
#         # # 데이터 읽기
#         # rx_data = await client.read_gatt_char(uuid_rx)
#         # print("수신 데이터:", rx_data)

#         # 데이터 쓰기
#         tx_data = b"\x53\x02\xE8\x03\xFF\x45"  # 쓸 데이터
#         await client.start_notify(uuid_tx, notification_handler)
#         await asyncio.sleep(0.5)
#         await client.write_gatt_char(uuid_tx, tx_data, response=True)
#         await asyncio.sleep(0.5)
#         #await client.stop_notify(uuid_tx)
#         print("데이터 쓰기 완료:", tx_data)
#         while True:
#             # 데이터 읽기
#             rx_data = await client.read_gatt_char(uuid_rx)
#             print("수신 데이터:", rx_data)
#             await asyncio.sleep(1)

# if __name__ == "__main__":
#     loop = asyncio.get_event_loop()
#     loop.run_until_complete(communicate_with_ble_device())


# address = '90:38:0c:fe:bd:d2'
# address2 = '90:38:0C:FE:BD:22'
# notity_charcteristic_uuid = '6E400003-B5A3-F393-E0A9-E50E24DCCA9A'
# uuid_tx = '6E400002-B5A3-F393-E0A9-E50E24DCCA9A'
# service_uuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9A"

# import asyncio
# from bleak import BleakClient

# async def run(address):    
#     async with BleakClient(address) as client:
#         print('connected')
#         # 연결한 장치의 서비스 정보 얻기
#         services = await client.get_services()
#         # 리턴 받은 services의 타입 확인
#         print("Services:", type(services))
#         # 루프를 돌면서 출력
#         for service in services:
#             print(service)
#     print('disconnect')

# loop = asyncio.get_event_loop()
# loop.run_until_complete(run(address))
# print('done')

address = '90:38:0c:fe:bd:d2'
address2 = '90:38:0c:fe:bd:22'#
hex_data = '5302E803FF45'
notity_charcteristic_uuid = '6e400003-b5a3-f393-e0a9-e50e24dcca9a'
uuid_tx = '6e400002-b5a3-f393-e0a9-e50e24dcca9a'
service_uuid = '6e400001-b5a3-f393-e0a9-e50e24dcca9a'

import asyncio
from bleak import BleakClient
from PyQt5.QtWidgets import QApplication, QMainWindow
import qasync
from PyQt5.QtCore import QObject, pyqtSignal
class DataReceiver(QObject):
    data_received = pyqtSignal(bytearray)

    def handle_received_data(self, data):
        print("--------------------------",data)

data_receiver = DataReceiver()
data_receiver.data_received.connect(data_receiver.handle_received_data)
def notify_callback(sender: int, data: bytearray):
    print('sender: ', sender, 'data: ', data)
    data_receiver.data_received.emit(data)

async def connect(address):
    # BleakClient 클래스 생성 및 바로 연결 시작
    # address: ESP32 맥주소
    # timeout: 연결 제한 시간 5초가 넘어가면 더 이상 연결하지 말고 종료
    async with BleakClient(address, timeout=5.0) as client:                    
        # 연결을 성공함
        print('connected')
        # 연결된 BLE 장치의 서비스 요청
        services = await client.get_services()
        # 서비스들을 루프돌려 내부 캐릭터리스틱 정보 조회
        for service in services:
            print('service uuid:', service.uuid)
            # 각 서비스들에 있는 캐릭터리스틱을 루프 돌려 속성들 파악하기
            for characteristic in service.characteristics:
                print('  uuid:', characteristic.uuid)
                # handle 정보도 함께 확인
                print('  handle:', characteristic.handle) 
                print('  properties: ', characteristic.properties)
                # 캐릭터리스틱 UUID가 우리가 찾는 UUID인지 먼저 확인
                #if str(characteristic.uuid) == uuid_tx:  
                if 'notify' in characteristic.properties:
                    # notify 속성이 있다면 BLE 장치의 notify 속성을 
                    # 활성화 작업 후 notify_callback 함수 연결                        
                    print('try to activate notify.')
                    await client.start_notify(characteristic, notify_callback)
                    print('존재한다')
                    #b"\x53\x03\xE8\x03\xFF\x45"
                    # client 가 연결된 상태라면        
        if client.is_connected:
            print("클라이언트 연결")
            # 1초간 대기
            d = "5303e803ff45"
            data_to_write = bytearray.fromhex(d)
            await client.write_gatt_char(characteristic.uuid,data_to_write,response=True)
                        #await asyncio.sleep(40) 
                        #print('try to deactivate notify.')
                        # 활성시켰단 notify를 중지 시킨다.
                        #await client.stop_notify(notity_charcteristic_uuid)
                    #await client.read_gatt_char(notity_charcteristic_uuid)
                    #await client.write_gatt_char(uuid_tx,b"\x53\x03\xE8\x03\xFF\x45",response=True)
                    # 우리가 찾던 UUID가 맞다면 
                    # 해당 캐릭터리스틱에 notify 속성이 있는지 확인
                    # notify 속성이 있다면 BLE 장치의 notify 속성을 
                    # 활성화 작업 후 notify_callback 함수 연결                        
                    #print('try to activate notify.')
                    #await client.start_notify(notity_charcteristic_uuid, notify_callback)
        

    print('disconnect')

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()


if __name__ == "__main__":
    asyncio.run(connect(address))
    
# import win32com.client
# import time

# # COM 포트 설정
# com_port = "COM15"  # 실제 사용하는 COM 포트로 변경

# try:
#     # Bluetooth 개체 생성
#     bt = win32com.client.Dispatch("Bluetooth.Factory1")

#     # RFComm 채널 연결
#     service = bt.GetModule(service_uuid)
#     port = service[com_port]

#     while True:
#         # 데이터 전송
#         data_to_send = b"\x53\x02\xE8\x03\xFF\x45"
#         port.Write(data_to_send)

#         # 데이터 수신
#         received_data = port.Read(1024).decode()
#         print(f"Received data: {received_data}")

#         # 100ms 대기
#         time.sleep(0.1)

# except Exception as e:
#     print(f"Error: {e}")
