from PySide6.QtWidgets import QWidget,QVBoxLayout
import numpy as np
import pyqtgraph as pg
from pyside_viewer import Ui_viewer
from PySide6.QtGui import QIcon
from PySide6.QtCore import Qt,QSize

#히트맵 색깔 범위 
H_MIN = 1
H_MAX = 40
#저장 시간 
TIME = 1000

class visual_widget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_viewer()
        self.ui.setupUi(self)
        #icon
        conn_icon = QIcon("connect.png")
        
        not_conn_icon = QIcon("not_connect.png")
        
        self.ui.left_btn.setIcon(conn_icon)
        self.ui.left_btn.setIconSize(QSize(64,64))
        self.ui.right_btn.setIcon(conn_icon)
        self.ui.right_btn.setIconSize(QSize(64,64))
        #환경 저장
        #self.db = ats_database.connectdb()
        #히트맵
        self.plot_widget_left = pg.PlotWidget(self.ui.left)
        self.plot_widget_right = pg.PlotWidget(self.ui.right)
        self.plot_widget_left.setAspectLocked(False)
        self.plot_widget_right.setAspectLocked(False)
        r_x_range = (1, 6)
        r_y_range = (0.5, 6)
        l_x_range = (1, 6)
        l_y_range = (0.5, 6)
        self.plot_widget_left.getPlotItem().setXRange(*l_x_range)
        self.plot_widget_left.getPlotItem().setYRange(*l_y_range)
        self.plot_widget_right.getPlotItem().setXRange(*r_x_range)
        self.plot_widget_right.getPlotItem().setYRange(*r_y_range)
        self.color= [
                        [0, 0, 255], [0, 36, 218], [0, 72, 182], [0, 109, 145], [0, 145, 109], 
                        [0, 182, 72], [0, 218, 36], [0, 255, 0], [0, 255, 0], [36, 255, 0], 
                        [72, 255, 0], [109, 255, 0], [145, 255, 0], [182, 255, 0], [218, 255, 0], 
                        [255, 255, 0], [255, 255, 0], [255, 242, 0], [255, 229, 0], [255, 216, 0], 
                        [255, 203, 0], [255, 190, 0], [255, 177, 0], [255, 165, 0], [255, 165, 0], 
                        [255, 141, 0], [255, 117, 0], [255, 94, 0], [255, 70, 0], [255, 47, 0], 
                        [255, 23, 0], [255, 0, 0]
                    ]
        # Left plot
        self.cmap_left = pg.ColorMap(pos=np.linspace(0.0, 1.0, 32), color=self.color)
        self.heatmap_left = pg.ImageItem()
        self.vbox_left = QVBoxLayout()
        self.heatmap_left.setLookupTable(self.cmap_left.getLookupTable())
        self.plot_widget_left.addItem(self.heatmap_left)
        self.vbox_left.addWidget(self.plot_widget_left)
        self.ui.left.setLayout(self.vbox_left)

        # Right plot
        self.cmap_right = pg.ColorMap(pos=np.linspace(0.0, 1.0, 32), color=self.color)
        self.heatmap_right = pg.ImageItem()
        self.vbox_right = QVBoxLayout()
        self.heatmap_right.setLookupTable(self.cmap_right.getLookupTable())
        self.plot_widget_right.addItem(self.heatmap_right)
        self.vbox_right.addWidget(self.plot_widget_right)
        self.ui.right.setLayout(self.vbox_right)

        # guide_data를 0으로 초기화
        guide_data = np.zeros((4, 4))

        # 원하는 영역에 값을 할당
        guide_data[0:2, 0:2] = 5
        guide_data[2:4, 2:4] = 10
        guide_data[0:2, 2:4] = 15
        guide_data[2:4, 0:2] = 20

         # guide_data를 0으로 초기화
        guide_data2 = np.zeros((4, 4))

        # 원하는 영역에 값을 할당
        guide_data2[0:2, 0:2] = 30
        guide_data2[2:4, 2:4] = 25
        guide_data2[0:2, 2:4] = 20
        guide_data2[2:4, 0:2] = 15
        
        # data_dict에서 주어진 주소에 해당하는 데이터를 가져옴
        # guide_data[0:2, 0:2] = 5 
        # guide_data[2:4, 2:4] = 10
        # guide_data[0:2, 2:4] = 15
        # guide_data[2:4, 0:2] = 20
        
        self.heatmap_left.setImage(guide_data)
        self.heatmap_left.setLevels([H_MIN,H_MAX])
        self.heatmap_right.setImage(guide_data2)
        self.heatmap_right.setLevels([H_MIN,H_MAX])

        text1 = pg.TextItem( text=f'{5:.1f}', color=(0, 0, 0)) 
        text1.setPos(1,1)
        self.plot_widget_left.addItem(text1)

        text2 = pg.TextItem( text=f'{10:.1f}', color=(0, 0, 0)) 
        text2.setPos(3,3)
        self.plot_widget_left.addItem(text2)

        text3 = pg.TextItem( text=f'{15:.1f}', color=(0, 0, 0)) 
        text3.setPos(1,3)
        self.plot_widget_left.addItem(text3)

        text4 = pg.TextItem( text=f'{20:.1f}', color=(0, 0, 0)) 
        text4.setPos(3,1)
        self.plot_widget_left.addItem(text4)

        text5 = pg.TextItem( text=f'{30:.1f}', color=(0, 0, 0)) 
        text5.setPos(1,1)
        self.plot_widget_right.addItem(text5)

        text6 = pg.TextItem( text=f'{25:.1f}', color=(0, 0, 0)) 
        text6.setPos(3,3)
        self.plot_widget_right.addItem(text6)

        text7 = pg.TextItem( text=f'{20:.1f}', color=(0, 0, 0)) 
        text7.setPos(1,3)
        self.plot_widget_right.addItem(text7)

        text8 = pg.TextItem( text=f'{15:.1f}', color=(0, 0, 0)) 
        text8.setPos(3,1)
        self.plot_widget_right.addItem(text8)

        



# class visual_widget(QWidget):
#     def __init__(self, parent=None):
#         super().__init__(parent)
#         self.ui = Ui_viewer()
#         self.ui.setupUi(self)
#         #환경 저장
#         #self.db = ats_database.connectdb()
#         #히트맵
#         self.plot_widget_left = pg.PlotWidget(self.ui.left)
#         self.plot_widget_right = pg.PlotWidget(self.ui.right)
#         self.plot_widget_left.setAspectLocked(False)
#         self.plot_widget_right.setAspectLocked(False)
#         r_x_range = (1, 6)
#         r_y_range = (0.5, 6)
#         l_x_range = (1, 6)
#         l_y_range = (0.5, 6)
#         self.plot_widget_left.getPlotItem().setXRange(*l_x_range)
#         self.plot_widget_left.getPlotItem().setYRange(*l_y_range)
#         self.plot_widget_right.getPlotItem().setXRange(*r_x_range)
#         self.plot_widget_right.getPlotItem().setYRange(*r_y_range)
#         self.color= [
#                         [0, 0, 255], [0, 36, 218], [0, 72, 182], [0, 109, 145], [0, 145, 109], 
#                         [0, 182, 72], [0, 218, 36], [0, 255, 0], [0, 255, 0], [36, 255, 0], 
#                         [72, 255, 0], [109, 255, 0], [145, 255, 0], [182, 255, 0], [218, 255, 0], 
#                         [255, 255, 0], [255, 255, 0], [255, 242, 0], [255, 229, 0], [255, 216, 0], 
#                         [255, 203, 0], [255, 190, 0], [255, 177, 0], [255, 165, 0], [255, 165, 0], 
#                         [255, 141, 0], [255, 117, 0], [255, 94, 0], [255, 70, 0], [255, 47, 0], 
#                         [255, 23, 0], [255, 0, 0]
#                     ]
#         # Left plot
#         self.cmap_left = pg.ColorMap(pos=np.linspace(0.0, 1.0, 32), color=self.color)
#         self.heatmap_left = pg.ImageItem()
#         self.vbox_left = QVBoxLayout()
#         self.heatmap_left.setLookupTable(self.cmap_left.getLookupTable())
#         self.plot_widget_left.addItem(self.heatmap_left)
#         self.vbox_left.addWidget(self.plot_widget_left)
#         self.ui.left.setLayout(self.vbox_left)

#         # Right plot
#         self.cmap_right = pg.ColorMap(pos=np.linspace(0.0, 1.0, 32), color=self.color)
#         self.heatmap_right = pg.ImageItem()
#         self.vbox_right = QVBoxLayout()
#         self.heatmap_right.setLookupTable(self.cmap_right.getLookupTable())
#         self.plot_widget_right.addItem(self.heatmap_right)
#         self.vbox_right.addWidget(self.plot_widget_right)
#         self.ui.right.setLayout(self.vbox_right)

#         self.timer = QTimer()
    
#     #히트맵 그리는 시작 타이머
#     def start_timer(self):
#         self.timer.timeout.connect(self.update_heatmap)
#         self.timer.start(TIME)
    
#     #히트맵 끝내는 타이머
#     def stop_timer(self):
#         self.timer.stop()

#     def update_heatmap(self):
#         global g_data_dict
#         #히트맵 위젯 생성
#         dict_data = g_data_dict
#         number = []
#         address_list = []
#         if len(dict_data) > 0:

#             # 이전에 추가된 텍스트 아이템 제거
#             for item in self.plot_widget.items():
#                 if isinstance(item, pg.TextItem):
#                     self.plot_widget.removeItem(item)
            
#             guide_data = np.zeros((7*2,7*2))
#             for i in range(len(self.check_address)):
#                 for j in range(len(self.check_address[i])):
#                     address = self.check_address[i][j]
#                     if address in dict_data:
#                         # 각 셀에 대한 인덱스 계산
#                         row_start = i * 4
#                         row_end = (i + 1) * 4
#                         col_start = j * 2
#                         col_end = (j + 1) * 2
#                         # data_dict에서 주어진 주소에 해당하는 데이터를 가져옴
#                         data_ = dict_data[address]
#                         guide_data[col_start:col_end, row_start:row_end] = data_ 
#                         number.append(data_)
#                         address_list.append(address)
            
#             #self.heatmap.clear()
#             self.heatmap.setImage(guide_data)
#             self.heatmap.setLevels([H_MIN,H_MAX])

    
#     @pyqtSlot(bool)
#     def rec_btn_click(self,state):#녹화버튼
#         global is_rec
#         self.ui.vr_save_pressure_btn.setText({True:"중지!",False:"녹화!"}[state])
#         is_rec = state
#         if is_rec:
#             self.first_env()

#     #first_env와 update_env 분리해 놓은 이유가 있음
#     #is_reset 트리거 관리로 인해 분리해 놓았으니 합치지 말것 
#     def first_env(self):#처음 환경 정보 
#         if self.is_new_env:
#             self.is_new_env = False
    


