address = '90:38:0c:fe:bd:d2'
address2 = '90:38:0c:fe:bd:22'
hex_data = '5302E803FF45'
notity_charcteristic_uuid = '6e400003-b5a3-f393-e0a9-e50e24dcca9a'
uuid_tx = '6e400002-b5a3-f393-e0a9-e50e24dcca9a'
service_uuid = '6e400001-b5a3-f393-e0a9-e50e24dcca9a'

import asyncio
import sys
import qasync
from bleak import BleakClient
from dataclasses import dataclass
from functools import cached_property
from PyQt5.QtCore import QObject, pyqtSignal
from PyQt5.QtWidgets import (
    QApplication,
    QLineEdit,
    QMainWindow,
    QPlainTextEdit,
    QPushButton,
    QWidget,
    QVBoxLayout
)

@dataclass
class Neo_BleakClinet(QObject):
    address: str

    def __post_init__(self):
        super().__init__()
        
    def notify_callback(self,sender: int, data: bytearray):
        print('sender: ', sender, 'data: ', data)

    @cached_property
    def client(self) -> BleakClient:
        return BleakClient(self.address, disconnected_callback=self._handle_disconnect)
    
    def _handle_disconnect(self) -> None:
        print("Device was disconnected, goodbye.")
        for task in asyncio.all_tasks():
            task.cancel()
    
    async def disconnect(self):
        await self.client.disconnect()
            
    async def connect(self):
        # BleakClient 클래스 생성 및 바로 연결 시작
        # address: ESP32 맥주소
        # timeout: 연결 제한 시간 5초가 넘어가면 더 이상 연결하지 말고 종료
        async with BleakClient(self.address, timeout=5.0) as client:                    
            # 연결을 성공함
            print('connected')
            # 연결된 BLE 장치의 서비스 요청
            services = await client.get_services()
            # 서비스들을 루프돌려 내부 캐릭터리스틱 정보 조회
            for service in services:
                for characteristic in service.characteristics:
                    if 'notify' in characteristic.properties:  
                        print('try active notify')
                        await client.start_notify(characteristic,self.notify_callback)
                        print("enable notify")

    async def write(self,data):
            # client 가 연결된 상태라면        
            if self.client.is_connected:
                data_to_send = bytearray.fromhex(data)
                await self.client.write_gatt_char('6E400002-B5A3-F393-E0A9-E50E24DCCA9A',data_to_send,response=True)
                
                # 활성시켰단 notify를 중지 시킨다.
                #await client.stop_notify(notity_charcteristic_uuid)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.resize(640, 480)

        self._client = None
        self.num = 0
        connect_button = QPushButton("Connect")
        self.message_lineedit = QLineEdit()
        send_button = QPushButton("Send Message")
        self.log_edit = QPlainTextEdit()

        self.address = '90:38:0c:fe:bd:d2'
        self.address2 = '90:38:0c:fe:bd:22'
        
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        lay = QVBoxLayout(central_widget)
        lay.addWidget(connect_button)
        lay.addWidget(self.message_lineedit)
        lay.addWidget(send_button)
        lay.addWidget(self.log_edit)
        
        connect_button.clicked.connect(self.handle_connect)
        send_button.clicked.connect(self.handle_send)

    @cached_property
    def devices(self):
        return list()

    @property
    def current_client(self):
        return self._client

    @qasync.asyncSlot()
    async def handle_connect(self):
        if self._client is not None:
            await self._client.disconnect()
        self._client = Neo_BleakClinet(self.address2)
        await self._client.connect()
        self.log_edit.appendPlainText("try connect")
        
    def handle_message_changed(self, message):
        self.log_edit.appendPlainText(f"msg: {message.decode()}")
        
    @qasync.asyncSlot()
    async def handle_send(self):
        if self.current_client is None:
            return
        message = bytearray.fromhex(self.message_lineedit.text())
        if message:
            await self.current_client.write(message)

def main():
    app = QApplication(sys.argv)
    loop = qasync.QEventLoop(app)
    asyncio.set_event_loop(loop)
    w = MainWindow()
    w.show()
    with loop:
        loop.run_forever()


if __name__ == "__main__":
    main()