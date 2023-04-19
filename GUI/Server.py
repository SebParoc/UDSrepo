import sys
from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtCore import QThread, pyqtSignal


import subprocess
path = "./../cmake-build-debug-visual-studio/Debug/server.exe"


class WorkerThread(QThread):
    signal = pyqtSignal(str)

    def __init__(self):
        QThread.__init__(self)

    def run(self):

        server = subprocess.Popen([path], stdout=subprocess.PIPE)

        while True:
            output = server.stdout.readline()
            if output:
                self.signal.emit(output.decode("utf-8").strip())
            else:
                break

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        font = QtGui.QFont('Arial', 12)

        self.serverLabel = QtWidgets.QLabel("Server open")
        self.serverLabel.setFont(font)
        self.serverLabel.setAlignment(QtCore.Qt.AlignCenter)
        self.output_text = QtWidgets.QTextEdit()
        self.output_text.setFont(font)
        self.output_text.setReadOnly(True)

        bg_color = QtGui.QColor(40, 44, 52)
        text_color = QtGui.QColor(255, 255, 255)

        self.output_text.setStyleSheet(f"background-color: {bg_color.name()}; color: {text_color.name()}; border: 2px "
                                      f"solid #4d4d4d; border-radius: 10px; padding: 5px;")

        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.serverLabel)
        layout.addWidget(self.output_text)

        central_widget = QtWidgets.QWidget()
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

        self.workerThread = WorkerThread()
        self.workerThread.signal.connect(self.appendText)
        self.workerThread.start()

    def appendText(self, text):
        self.output_text.append(text)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)

    app.setStyle('Fusion')

    palette = QtGui.QPalette()
    palette.setColor(QtGui.QPalette.Window, QtGui.QColor(40, 44, 52))
    palette.setColor(QtGui.QPalette.WindowText, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.Base, QtGui.QColor(28, 32, 38))
    palette.setColor(QtGui.QPalette.AlternateBase, QtGui.QColor(44, 49, 58))
    palette.setColor(QtGui.QPalette.ToolTipBase, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.ToolTipText, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.Text, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.Button, QtGui.QColor(28, 32, 38))
    palette.setColor(QtGui.QPalette.ButtonText, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.BrightText, QtGui.QColor(255, 0, 0))
    palette.setColor(QtGui.QPalette.Highlight, QtGui.QColor(255, 140, 0))
    palette.setColor(QtGui.QPalette.HighlightedText, QtGui.QColor(255, 255, 255))
    app.setPalette(palette)

    window = MainWindow()
    window.show()
    sys.exit(app.exec_())