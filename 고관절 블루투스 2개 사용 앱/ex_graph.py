import sys
import numpy as np
from PySide6.QtWidgets import QApplication, QMainWindow, QGridLayout, QWidget
from PySide6.QtCore import Qt

def main():
    app = QApplication(sys.argv)
    window = QMainWindow()
    window.setGeometry(100, 100, 800, 600)
    window.setWindowTitle("히트맵 그래프 예제")

    central_widget = QWidget()
    window.setCentralWidget(central_widget)

    # QCustomPlot 히트맵 그래프 생성
    custom_plot = QCustomPlot(central_widget)
    
    # 데이터 생성 (예시)
    data = np.random.rand(10, 10)
    
    # 히트맵 데이터 설정
    custom_plot.addGraph()
    custom_plot.graph(0).data().setArray(data)
    custom_plot.rescaleAxes()
    
    # 화면에 그리기
    layout = QGridLayout(central_widget)
    layout.addWidget(custom_plot, 0, 0)
    central_widget.setLayout(layout)
    
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
