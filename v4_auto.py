# importing libraries
import subprocess as sp
from stopwatch import Stopwatch
from PyQt5.QtWidgets import * 
from PyQt5 import QtCore, QtGui
from PyQt5.QtGui import * 
from PyQt5.QtCore import * 
from PyQt5.QtGui import QIcon
from PyQt5 import uic
from datetime import datetime
import pyautogui
import keyboard
import random
import time
import sys

#종료시간 18시
exit_time = 18

class Run_Thread(QThread):
    #parent = MainWidget을 상속 받음.
    def __init__(self, parent):
        super().__init__(parent)
        
    def run(self):
        while True:
           
   
            # 현재 시간
            current_time = datetime.now()
            
         
            # ----------------------------------------------------------
            #version 4 update
            #C:\Windows\System32\notepad.exe Run
            programName = "notepad.exe"
            sp.Popen([programName, ""])
            time.sleep(1)
            # 메모장 클릭
            # ----------------------------------------------------------

            # ----------------------------------------------------------
            pyautogui.click(x=917,y=597,button='left')
            time.sleep(1)

            # 메모장 write
            pyautogui.typewrite('Lorem ipsum dolor sit amet,consectetuer\n'
                                'adipiscing elit. Aenean commodo ligula eget dolor.\n'
                                'Aenean massa. Cum sociis natoque penatibus et\n'
                                'magnis dis parturient montes, nascetur ridiculus mus.\n'
                                'Donec quam felis, ultricies nec, pellentesque eu, pretium quis,\n'
                                'sodales sagittis magna. Sed consequat, leo eget bibendum sodales,\n'
                                'augue velit cursus nunc\n',interval=0.1)
            
            time.sleep(3)
            pyautogui.typewrite('\nExit this program\nbye!',interval=0.1)
            time.sleep(3)
            # ----------------------------------------------------------

          
            
            # ----------------------------------------------------------
            #exit notepad.exe
            CREATE_NO_WINDOW = 0x08000000
            sp.call('taskkill /F /IM notepad.exe', creationflags=CREATE_NO_WINDOW)
            time.sleep(2)

            
            if current_time.hour == exit_time:
                exit(0)
            # ----------------------------------------------------------

class Window(QMainWindow):
  
    def __init__(self):
        super().__init__()
        # setting title
        self.setWindowTitle("마우스 자동화")
        # setting geometry
        self.setGeometry(100, 100, 400, 500)
        # set resize
        self.resize(400,400)
        # calling method
        self.InitUi()
        # showing all the widgets
        self.show()
  
    # method for widgets
    def InitUi(self):

        # creating timer 
        self.count  = 0
        self.m_ms   = 0
        self.m_sec  = 0
        self.m_min  = 0
        self.m_hour = 0
        # creating flag
        self.flag = False
        # creating a label to show the time
        self.label = QLabel(self)

        # ----------------------------------------------------------
        # setting geometry of label position 
        self.label.setGeometry(75, 60, 250, 70)
        # adding border to the label  (rectangle box)
        self.label.setStyleSheet("border: 2px solid rgb(58, 134, 255);")
        # setting text to the label
        self.label.setText(str(self.count))
        # setting font and size to the label 
        self.label.setFont(QFont('Arial', 20))
        # setting alignment to the text of label
        self.label.setAlignment(Qt.AlignCenter)
        # ----------------------------------------------------------

        # ----------------------------------------------------------
        # creating start_timer button
        btn_start_timer = QPushButton("timer", self)
        # setting geometry to the button
        btn_start_timer.setGeometry(75, 10, 125, 40)
        # add action to the method
        btn_start_timer.pressed.connect(self.Start_Timer)
        # setting style to the button
        btn_start_timer.setStyleSheet("color : rgb(58, 134, 255);"
                                    "background-color: white;"
                                    "border: 2px solid rgb(58, 134, 255);"
                                    "border-radius: 5px;")
        # ----------------------------------------------------------


        # ----------------------------------------------------------
        # creating pause button
        btn_pause_timer = QPushButton("Pause", self)
        btn_pause_timer.setGeometry(200, 10, 125, 40)
        btn_pause_timer.pressed.connect(self.Pause_Timer)
        btn_pause_timer.setStyleSheet("color : rgb(58, 134, 255);"
                                    "background-color: white;"
                                    "border: 2px solid rgb(58, 134, 255);"
                                    "border-radius: 5px;")
        # ----------------------------------------------------------


        # ----------------------------------------------------------
         # creating run thread button
        btn_run_thread = QPushButton("자동화 시작!",self)
        btn_run_thread.setGeometry(95, 170, 205, 40)
        btn_run_thread.pressed.connect(self.run_thread)
        btn_run_thread.setStyleSheet("color: white;"
	"background-color:qlineargradient(spread:reflect, x1:1, y1:0, x2:0.995, y2:1, stop:0 rgba(218, 218, 218, 255), stop:0.305419 rgba(0, 7, 11, 255), stop:0.935961 rgba(2, 11, 18, 255), stop:1 rgba(240, 240, 240, 255));"
	"border: 1px solid black;"
	"border-radius: 20px")
        # ----------------------------------------------------------


        # ----------------------------------------------------------
        # label update  할 예정(진행률)
        # ----------------------------------------------------------


        # ----------------------------------------------------------
        # creating exit button
        btn_exit = QPushButton("종료!", self)
        btn_exit.setGeometry(95, 350, 205, 40)
        btn_exit.pressed.connect(self.Exit_Process)
        btn_exit.setStyleSheet("color: white;"
	"background-color:qlineargradient(spread:reflect, x1:1, y1:0, x2:0.995, y2:1, stop:0 rgba(218, 218, 218, 255), stop:0.305419 rgba(0, 7, 11, 255), stop:0.935961 rgba(2, 11, 18, 255), stop:1 rgba(240, 240, 240, 255));"
	"border: 1px solid black;"
	"border-radius: 20px")
        # ----------------------------------------------------------


        # ----------------------------------------------------------
        # creating a timer object
        timer = QTimer(self)
        # adding action to timer
        timer.timeout.connect(self.showTime)
        # update the timer every tenth second
        timer.start(100)
        # ----------------------------------------------------------


    # ----------------------------------------------------------
    # method called by run thread
    def run_thread(self):
        x = Run_Thread(self)
        x.start()
    # ----------------------------------------------------------

    # ----------------------------------------------------------
    # method called by timer
    def showTime(self):
        # checking if flag is true
        if self.flag:
            self.m_ms+= 1
                 
        self.m_sec = self.m_ms/10

        if self.m_sec == 60:
            self.m_sec=0
            self.m_ms=1
            self.m_min+=1
        
        if self.m_min == 60:
            self.m_min=0
            self.m_hour+=1
        
        text = "%d시 %d분 %.1f초"%(self.m_hour,self.m_min,self.m_sec)
        
        # showing text  
        self.label.setText(str(text)) 
    # ----------------------------------------------------------
  
    # ----------------------------------------------------------
    def Start_Timer(self):
        # making flag to true
        self.flag = True
    # ----------------------------------------------------------

    # ----------------------------------------------------------
    def Pause_Timer(self):
        # making flag to false
        self.flag = False
    # ----------------------------------------------------------

    # ----------------------------------------------------------
    def Exit_Process(self):
        time.sleep(1)
        # write log update 예정(timer 기준)
        sys.exit()
        #-----------deprecated--------------
        # setting text to label
        self.label.setText(str(self.count))
        #------------------------------------
    # ----------------------------------------------------------
  


# ----------------------------------------------------------
# create pyqt5 app
App = QApplication(sys.argv)
# create the instance of our Window
window = Window()
# start the app
sys.exit(App.exec())
# ----------------------------------------------------------
