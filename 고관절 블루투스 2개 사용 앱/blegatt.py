from PySide6.QtBluetooth import QLowEnergyController
from PySide6.QtCore import QPoint, Qt, Slot
from PySide6.QtGui import QColor
from PySide6.QtBluetooth import (QBluetoothDeviceDiscoveryAgent, QBluetoothLocalDevice,
                                 QBluetoothAddress, QBluetoothUuid,QBluetoothDeviceInfo,
                                 QLowEnergyServiceData,QLowEnergySer)
from PySide6.QtWidgets import QWidget,QListWidgetItem

SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9A"
CHARACTERISTIC_UUID_RX = "6E400002-B5A3-F393-E0A9-E50E24DCCA9A"
CHARACTERISTIC_UUID_TX = "6E400003-B5A3-F393-E0A9-E50E24DCCA9A"

class ble_gatt(QWidget):
    def __init__(self):
        super().__init__()

        self.GATT_server = Qlow

        self.controller1 = QLowEnergyController.createCentral()
        self.controller2 = QLowEnergyController.createCentral()
        self._discovery_agent = QBluetoothDeviceDiscoveryAgent()

        self._ui.scan.clicked.connect(self.start_scan) #장치검색
        self._discovery_agent.deviceDiscovered.connect(self.add_device) #검색된 장치목록 생성

    @Slot()
    def start_scan(self):
        self._discovery_agent.start()
        self._ui.scan.setEnabled(False)
    
    @Slot(QBluetoothDeviceInfo)
    def add_device(self, info):
        a = info.address().toString()
        label = f"{a} {info.name()}"
        items = self._ui.list.findItems(label, Qt.MatchExactly)
        if not items:
            item = QListWidgetItem(label)
            pairing_status = self._local_device.pairingStatus(info.address())
            if pairing_status == QBluetoothLocalDevice.Paired:
                item.setForeground(QColor(Qt.green))
            else:
                item.setForeground(QColor(Qt.black))
            self._ui.list.addItem(item)


    def device_discovered(self,device_info):
        # 검색된 BLE 장치 정보 중에서 서비스 UUID 추출
        service_uuid = QBluetoothUuid("0000180d-0000-1000-8000-00805f9b34fb")  # 예시 UUID
        device_service_uuids = device_info.serviceUuids()

        if service_uuid in device_service_uuids:
            # 원하는 서비스 UUID를 가진 장치를 찾았을 때
            device_address = device_info.address().toString()
            print(f"Found device with the desired service UUID: {device_address}")

        address = ""#"XX:XX:XX:XX:XX:XX"  # BLE 장치의 주소
        #connect
        connect1 = QLowEnergyServiceData.addIncludedService()
        connect1.setRemoteAddress(address)
        self.controller1.connectToDevice()
        service_uuid = "0000180d-0000-1000-8000-00805f9b34fb"  # 예시 UUID
        self.controller1.serviceDiscovered.connect(self.service_discovered)


    def service_discovered(self,service_info):
        if service_info.uuid() == service_uuid:
            service = controller.createServiceObject(service_info)
            if service:
                service.stateChanged.connect(service_state_changed)
                service.characteristicChanged.connect(characteristic_changed)
                service.discoverDetails()


    def characteristic_changed(self,characteristic, value):
        # 특성 값이 변경될 때 호출됨
        print("Characteristic Value Changed:", value)

    def service_state_changed(state):
        if state == QLowEnergyService.ServiceDiscovered:
            service = controller.service()
            if service:
                characteristic = service.characteristic(QBluetoothUuid.CharacteristicType.HeartRateMeasurement)
                if characteristic:
                    # 특성 읽기
                    controller.readCharacteristic(characteristic)

    # 서비스 및 특성에 액세스하는 코드 추가
