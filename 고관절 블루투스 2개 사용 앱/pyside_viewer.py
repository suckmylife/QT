# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'pollyworks_viewer.ui'
##
## Created by: Qt User Interface Compiler version 6.5.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QFrame, QLabel, QPushButton,
    QSizePolicy, QWidget)

class Ui_viewer(object):
    def setupUi(self, viewer):
        if not viewer.objectName():
            viewer.setObjectName(u"viewer")
        viewer.resize(1099, 548)
        self.left = QFrame(viewer)
        self.left.setObjectName(u"left")
        self.left.setGeometry(QRect(30, 80, 501, 441))
        self.left.setFrameShape(QFrame.StyledPanel)
        self.left.setFrameShadow(QFrame.Raised)
        self.right = QFrame(viewer)
        self.right.setObjectName(u"right")
        self.right.setGeometry(QRect(560, 80, 501, 441))
        self.right.setFrameShape(QFrame.StyledPanel)
        self.right.setFrameShadow(QFrame.Raised)
        self.frame_3 = QFrame(viewer)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setGeometry(QRect(30, 20, 51, 51))
        self.frame_3.setFrameShape(QFrame.StyledPanel)
        self.frame_3.setFrameShadow(QFrame.Raised)
        self.frame_4 = QFrame(self.frame_3)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setGeometry(QRect(0, 0, 51, 51))
        self.frame_4.setFrameShape(QFrame.StyledPanel)
        self.frame_4.setFrameShadow(QFrame.Raised)
        self.left_btn = QPushButton(self.frame_4)
        self.left_btn.setObjectName(u"left_btn")
        self.left_btn.setGeometry(QRect(0, 0, 51, 51))
        self.frame_5 = QFrame(viewer)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setGeometry(QRect(560, 20, 51, 51))
        self.frame_5.setFrameShape(QFrame.StyledPanel)
        self.frame_5.setFrameShadow(QFrame.Raised)
        self.right_btn = QPushButton(self.frame_5)
        self.right_btn.setObjectName(u"right_btn")
        self.right_btn.setGeometry(QRect(0, 0, 51, 51))
        self.label = QLabel(viewer)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(210, 10, 141, 81))
        font = QFont()
        font.setFamilies([u"Arial Black"])
        font.setPointSize(36)
        font.setBold(True)
        self.label.setFont(font)
        self.label.setTextFormat(Qt.AutoText)
        self.label.setAlignment(Qt.AlignJustify|Qt.AlignVCenter)
        self.label_2 = QLabel(viewer)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setGeometry(QRect(730, 10, 181, 81))
        self.label_2.setFont(font)
        self.label_2.setTextFormat(Qt.AutoText)
        self.label_2.setAlignment(Qt.AlignJustify|Qt.AlignVCenter)

        self.retranslateUi(viewer)

        QMetaObject.connectSlotsByName(viewer)
    # setupUi

    def retranslateUi(self, viewer):
        viewer.setWindowTitle(QCoreApplication.translate("viewer", u"neoable viewer", None))
        self.left_btn.setText("")
        self.right_btn.setText("")
        self.label.setText(QCoreApplication.translate("viewer", u"LEFT", None))
        self.label_2.setText(QCoreApplication.translate("viewer", u"RIGHT", None))
    # retranslateUi

