from PySide6.QtCore import QPoint, Qt, Slot
from PySide6.QtGui import QColor
from PySide6.QtWidgets import QDialog, QListWidgetItem, QListWidget, QMenu
from PySide6.QtBluetooth import (QBluetoothAddress, QBluetoothDeviceDiscoveryAgent,
                                 QBluetoothDeviceInfo, QBluetoothLocalDevice,
                                 QBluetoothUuid)

from ui_device import Ui_DeviceDiscovery
from service import ServiceDiscoveryDialog

CHARACTERISTIC_UUID_RX = QBluetoothUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9A")
CHARACTERISTIC_UUID_TX = QBluetoothUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9A")

class DeviceDiscoveryDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        #self.service_uuid = QBluetoothUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9A")
        self.service_uuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9A"
        self._local_device = QBluetoothLocalDevice()
        self._ui = Ui_DeviceDiscovery()
        self._ui.setupUi(self)
        # In case of multiple Bluetooth adapters it is possible to set adapter
        # which will be used. Example code:
        #
        # address = QBluetoothAddress("XX:XX:XX:XX:XX:XX")
        # discoveryAgent = QBluetoothDeviceDiscoveryAgent(address)

        self._discovery_agent = QBluetoothDeviceDiscoveryAgent()

        self._ui.scan.clicked.connect(self.start_scan)
        self._discovery_agent.deviceDiscovered.connect(self.add_device)
        self._discovery_agent.finished.connect(self.scan_finished)
        self._ui.list.itemActivated.connect(self.item_activated)
        self._local_device.hostModeStateChanged.connect(self.host_mode_state_changed)

        self.host_mode_state_changed(self._local_device.hostMode())
        # add context menu for devices to be able to pair device
        self._ui.list.setContextMenuPolicy(Qt.CustomContextMenu)
        self._ui.list.customContextMenuRequested.connect(self.display_pairing_menu)
        self._local_device.pairingFinished.connect(self.pairing_done)

    @Slot(QBluetoothDeviceInfo)
    def add_device(self, info):
        a = info.address().toString()
        label = f"{a} {info.name()}"
        items = self._ui.list.findItems(label, Qt.MatchExactly)
        service_uuid = self.service_uuid
       
        if service_uuid in info.serviceUuids():
            print("Device with the desired service UUID found!")
            print("Device Address:", a)
        else:
            print("NOPE")
            print(a)
        if not items:
            item = QListWidgetItem(label)
            pairing_status = self._local_device.pairingStatus(info.address())
            if pairing_status == QBluetoothLocalDevice.Paired:
                item.setForeground(QColor(Qt.green))
            else:
                item.setForeground(QColor(Qt.black))
            self._ui.list.addItem(item)
        #service_uuid = QBluetoothUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9A")  # 예시 UUID
        


    @Slot()
    def start_scan(self):
        self._discovery_agent.start()
        self._ui.scan.setEnabled(False)

    @Slot()
    def scan_finished(self):
        self._ui.scan.setEnabled(True)

    @Slot(QListWidgetItem)
    def item_activated(self, item):
        text = item.text()
        index = text.find(' ')
        if index == -1:
            return

        address = QBluetoothAddress(text[0:index])
        name = text[index + 1:]

        d = ServiceDiscoveryDialog(name, address)
        d.exec()

    @Slot(bool)
    def on_discoverable_clicked(self, clicked):
        if clicked:
            self._local_device.setHostMode(QBluetoothLocalDevice.HostDiscoverable)
        else:
            self._local_device.setHostMode(QBluetoothLocalDevice.HostConnectable)

    @Slot(bool)
    def on_power_clicked(self, clicked):
        if clicked:
            self._local_device.powerOn()
        else:
            self._local_device.setHostMode(QBluetoothLocalDevice.HostPoweredOff)

    @Slot(QBluetoothLocalDevice.HostMode)
    def host_mode_state_changed(self, mode):
        self._ui.power.setChecked(mode != QBluetoothLocalDevice.HostPoweredOff)
        self._ui.discoverable.setChecked(mode == QBluetoothLocalDevice.HostDiscoverable)

        on = mode != QBluetoothLocalDevice.HostPoweredOff
        self._ui.scan.setEnabled(on)
        self._ui.discoverable.setEnabled(on)

    @Slot(QPoint)
    def display_pairing_menu(self, pos):
        if self._ui.list.count() == 0:
            return
        menu = QMenu(self)
        pair_action = menu.addAction("Pair")
        remove_pair_action = menu.addAction("Remove Pairing")
        chosen_action = menu.exec(self._ui.list.viewport().mapToGlobal(pos))
        current_item = self._ui.list.currentItem()

        text = current_item.text()
        index = text.find(' ')
        if index == -1:
            return

        address = QBluetoothAddress(text[0:index])
        if chosen_action == pair_action:
            self._local_device.requestPairing(address, QBluetoothLocalDevice.Paired)
        elif chosen_action == remove_pair_action:
            self._local_device.requestPairing(address, QBluetoothLocalDevice.Unpaired)

    @Slot(QBluetoothAddress, QBluetoothLocalDevice.Pairing)
    def pairing_done(self, address, pairing):
        items = self._ui.list.findItems(address.toString(), Qt.MatchContains)

        color = QColor(Qt.red)
        if pairing == QBluetoothLocalDevice.Paired or pairing == QBluetoothLocalDevice.AuthorizedPaired:
            color = QColor(Qt.green)
        for item in items:
            item.setForeground(color)


# from PySide6.QtCore import Qt
# from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QTextEdit, QVBoxLayout, QWidget
# from PySide6.QtBluetooth import QBluetoothDeviceDiscoveryAgent, QBluetoothSocket, QBluetoothUuid

# class BluetoothDataTransferApp(QMainWindow):
#     def __init__(self):
#         super().__init__()

#         self.setWindowTitle("Bluetooth Data Transfer")
#         self.central_widget = QWidget(self)
#         self.setCentralWidget(self.central_widget)

#         self.layout = QVBoxLayout(self.central_widget)

#         self.log_text = QTextEdit(self)
#         self.layout.addWidget(self.log_text)

#         self.connect_button = QPushButton("Connect", self)
#         self.layout.addWidget(self.connect_button)

#         self.connect_button.clicked.connect(self.connect_to_device)

#         self.device_discovery_agent = QBluetoothDeviceDiscoveryAgent(self)
#         self.device_discovery_agent.deviceDiscovered.connect(self.device_discovered)

#         self.socket = None

#     def device_discovered(self, device_info):
#         # 원하는 디바이스 선택 및 연결
#         if device_info.name() == "MyBluetoothDevice":
#             if QBluetoothUuid.YourServiceUuid in device_info.serviceUuids():
#                 self.socket = QBluetoothSocket(QBluetoothServiceInfo.RfcommProtocol)
#                 self.socket.connectToService(device_info.address(), QBluetoothUuid.YourServiceUuid)
#                 self.socket.connected.connect(self.on_device_connected)
    
#     def on_device_connected(self):
#         self.log_text.append("Connected to the device")

#         # 데이터 전송
#         data = "Hello, Bluetooth!"
#         self.socket.write(data.encode())

#         # 데이터 수신
#         self.socket.readyRead.connect(self.on_data_received)

#     def on_data_received(self):
#         data = self.socket.readAll().data().decode()
#         self.log_text.append(f"Received: {data}")

# if __name__ == "__main__":
#     app = QApplication([])

#     window = BluetoothDataTransferApp()
#     window.show()

#     app.exec()
