
from PyQt4 import QtCore, QtGui
import os
import re
import sys
import wmi
import time
import serial
import threading
import numpy as np
import pyqtgraph as pg
from   collections import deque
import serial.tools.list_ports

# Global variables

var = 0

#--------------------------#
UART_BUF_LENGTH = 1200     #
#--------------------------#


try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)



class Ui_Form(QtGui.QWidget):

    def __init__(self):
        QtGui.QWidget.__init__(self)
        QtGui.QMainWindow.__init__(self, None, QtCore.Qt.WindowStaysOnTopHint)
        self.setupUi(self)
        self.count = 0

        self.serialData = SerialData(UART_BUF_LENGTH)   


    def setupUi(self, Form):
        Form.setObjectName(_fromUtf8("Form"))
        Form.setEnabled(True)
        Form.resize(1000, 450)
        Form.setAcceptDrops(False)
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setSizeConstraint(QtGui.QLayout.SetDefaultConstraint)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.comboBox = QtGui.QComboBox(Form)
        self.comboBox.setObjectName(_fromUtf8("comboBox"))
        self.gridLayout.addWidget(self.comboBox, 0, 0, 1, 1)
        self.btnConnect = QtGui.QPushButton(Form)
        self.btnConnect.setObjectName(_fromUtf8("btnConnect"))
        self.gridLayout.addWidget(self.btnConnect, 0, 1, 1, 1)
        self.btnDisconnect = QtGui.QPushButton(Form)
        self.btnDisconnect.setEnabled(False)
        self.btnDisconnect.setObjectName(_fromUtf8("btnDisconnect"))
        self.gridLayout.addWidget(self.btnDisconnect, 0, 2, 1, 1)
        self.progressBar = QtGui.QProgressBar(Form)
        self.progressBar.setProperty("value", 0)
        self.progressBar.setObjectName(_fromUtf8("progressBar"))
        self.gridLayout.addWidget(self.progressBar, 0, 3, 1, 1)
        self.textBrowser = QtGui.QTextBrowser(Form)
        self.textBrowser.setObjectName(_fromUtf8("textBrowser"))
        self.gridLayout.addWidget(self.textBrowser, 1, 0, 1, 2)
        
        self.Graph = pg.GraphicsWindow(title="Accelerometer Graph")
        self.Graph.resize(650,450)
        self.Graph.setFrameShadow(QtGui.QFrame.Raised)
        self.Graph.setObjectName(_fromUtf8("Graph"))


        self.gridLayout.addWidget(self.Graph, 1, 2, 2, 2)
        self.sendData = QtGui.QLineEdit(Form)
        self.sendData.setObjectName(_fromUtf8("sendData"))
        self.gridLayout.addWidget(self.sendData, 2, 0, 1, 2)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)


    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Form", None))
        self.btnConnect.setText(_translate("Form", "Connect", None))
        self.btnDisconnect.setText(_translate("Form", "Disconnect", None))

        self.btnConnect.clicked.connect(self.serConnect)
        self.btnDisconnect.clicked.connect(self.serDisconnect)
        self.sendData.returnPressed.connect(self.transmitData)

        self.UpdateComPorts()
        self.AccGraphInitialize()


    # Transmit Serial Data

    def transmitData(self):

        if(var == 1):
            self.serialThread.sendData(self.sendData.text())

        else:
            self.updateLog("Cannot send data. No Active Serial Connection! ")


    # GUI Exit Event Handler

    def closeEvent(self, event):
        
        reply = QtGui.QMessageBox.question(self, 'Confirm Exit',
              "\n Are you sure to quit?  \n", QtGui.QMessageBox.Yes | 
                QtGui.QMessageBox.No, QtGui.QMessageBox.No)

        if reply == QtGui.QMessageBox.Yes:
            
            if(var == 1):    # Serial Disconnected?

                QtGui.QMessageBox.information(self, 'Serial Connected',
                                                    "\n Please Disconnect First.\n")

                print "Error: Serial Connection Active. Cannot Exit!"
                event.ignore() 

            else:
                print "Exiting Program..."
                os._exit(1)
                event.accept()

        else:
            event.ignore() 


    # Get Available Comports and Add to List

    def UpdateComPorts(self):

        query = "SELECT * FROM Win32_PnPEntity WHERE Name LIKE '%(COM%)'"
        coms  = wmi.WMI().query(query)

        self.updateLog("Available ComPorts: ")

        for com in coms:
            line = com.Name
            self.updateLog(com.Name)

            matchObj1 = re.match( r'.*(COM\d+).*', line, re.M|re.I)

            if matchObj1:
               self.comboBox.addItem(matchObj1.group(1))
            else:
               print "\nNo match!!"


    # GUI Connect event handler

    def serConnect(self):

        # Get Selected ComPort

        if(self.comboBox.currentText() == ""):
            
            QtGui.QMessageBox.information(self, 'Serial device missing',
                                             "\n Please connect your device first. GUI will exit now \t \n")

            self.updateLog("No ComPorts Available.... Exiting GUI.....")

            os._exit(1)

        else :
    
            self.btnConnect.setEnabled(False)
            self.btnDisconnect.setEnabled(True)

            print "You clicked the connect button"
            global var

            if(var != 1):

                var = 1
                self.serialThread = SerialThread(str(self.comboBox.currentText()))   
                self.serialThread.start() 

                self.connect(self.serialThread, QtCore.SIGNAL("serData(QString)"), self.updateLog)
                self.connect(self.serialThread, QtCore.SIGNAL("accData(QString, QString, QString)"), self.UpdateAcc)

                self.progressBar.setProperty("value", 100)


    # GUI Disconnect event handler

    def serDisconnect(self):

        self.btnConnect.setEnabled(True)
        self.btnDisconnect.setEnabled(False)
        
        global var  
        var = 2 

        print "You Clicked the Disconnect button"

        self.count = 0
        self.progressBar.setProperty("value", 0)


    # Update Command Log Text

    def updateLog(self, data):
        print data
        self.textBrowser.append(data)


    # Accelerometer Graph Intialization

    def AccGraphInitialize(self):

        pg.setConfigOptions(antialias=True)
        pg.setConfigOption('leftButtonPan', False)

        self.plot1  =  self.Graph.addPlot(title="Accelerometer Graph")

        self.plot1.addLegend()
        self.plot1.showGrid(x=True, y=True)

        self.curve1 =  self.plot1.plot(pen='r', name='G1')
        self.curve2 =  self.plot1.plot(pen='y', name='G2')


    # Update Accelerometer Graph

    def UpdateAcc(self, X, Y, Z):

        self.serialData.addData1(str(X))
        self.serialData.addData2(str(Y))

        self.count = self.count + 1;

        if(self.count >= UART_BUF_LENGTH):
            self.curve1.setData(np.array(self.serialData.data1).astype(np.float))
            self.curve2.setData(np.array(self.serialData.data2).astype(np.float))
            self.count = 0



# Class : SerialData
# func  : Create Data queues
# Input : Length of queue (MaxLength)
 
class SerialData:

    def __init__(self, MaxLength):

        self.data1 = deque([0.0] * MaxLength, maxlen=MaxLength)
        self.data2 = deque([0.0] * MaxLength, maxlen=MaxLength)
        self.data3 = deque([0.0] * MaxLength, maxlen=MaxLength)


        self.MaxLength = MaxLength

        print "SerialData Instance created"

    def __del__(self):
        print "SerialData Instance deleted"

    def addToBuf(self, buf, val):
        buf.appendleft(val)

    def addData1(self, val):
        self.addToBuf(self.data1, val)

    def addData2(self, val):
        self.addToBuf(self.data2, val)

    def addData3(self, val):
        self.addToBuf(self.data3, val)



# Class : SerialThread
# func  : Create serial instance
# Input : None

class SerialThread(QtCore.QThread):

    def __init__(self, comPort):

        QtCore.QThread.__init__(self)
        self.comPort = comPort
        print "SerialThread Instance created"


    def __del__(self):

        print "SerialThread Instance deleted"


    def sendData(self, data):

        string = list(data)

        for char in string:
            self.ser.write(str(char))


    def UpdateAccGraph(self, X, Y, Z):

        self.emit(QtCore.SIGNAL("accData(QString, QString, QString)"), X, Y, Z)

    def UpdateLogData(self, data):

        self.emit(QtCore.SIGNAL("serData(QString)"), data)


    def run(self):

        try:
            self.ser = serial.Serial(self.comPort, baudrate=115200, timeout=1)
        except serial.SerialException as e:
            print ("Could not open port '{}' : {}".format(str(self.comPort), e))
            os._exit(1)


        self.ser.write('1')    # Stop  UART TX
        self.ser.write('0')    # Start UART TX

        self.UpdateLogData("Connected to COM Port: " + str(self.comPort))
 
        while var == 1:
            try:
                line    =  self.ser.readline()

                mObj = re.match( r'(.*\d)A(.*\d).*', line, re.M|re.I)

                if mObj:
                    if mObj.group(1) != "" and mObj.group(2) != "":
                        self.UpdateAccGraph(mObj.group(1), mObj.group(2), str(0.0))
                

            except KeyboardInterrupt:
                print 'exiting'
                break

        else:
            self.ser.write('1')
            self.ser.flushInput()
            self.ser.close()

            self.UpdateLogData("Serial Terminal Closed")   



# Main 

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    ex  = Ui_Form()
    ex.show()
    sys.exit(app.exec_())

