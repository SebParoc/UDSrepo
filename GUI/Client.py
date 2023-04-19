import sys
import subprocess
from PyQt5 import QtWidgets, QtGui, QtCore

path = "./../cmake-build-debug-visual-studio/Debug/client.exe"
process = subprocess.Popen([path], stdin=subprocess.PIPE, stdout=subprocess.PIPE)


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        font = QtGui.QFont('Arial', 12)

        # Create a list to hold the line edits
        self.input_lines = []

        # Create 8 QLineEdit widgets and set their properties using a for loop
        for i in range(8):
            input_line = QtWidgets.QLineEdit()
            input_line.setFixedSize(40, 40)
            input_line.setFont(font)
            input_line.setPlaceholderText('ff')
            input_line.setAlignment(QtCore.Qt.AlignCenter)
            input_line.setValidator(QtGui.QRegExpValidator(QtCore.QRegExp('[0-9A-Fa-f]{0,2}')))
            input_line.setStyleSheet('border: 2px solid #4d4d4d; border-radius: 10px; padding: 5px;')
            self.input_lines.append(input_line)

        # Create a send button
        self.send_button = QtWidgets.QPushButton('Send')
        self.send_button.setFont(font)
        self.send_button.setStyleSheet('border: 2px solid #4d4d4d; border-radius: 10px; padding: 5px;')

        # Create an output text box
        self.output_text = QtWidgets.QPlainTextEdit()
        self.output_text.setFont(font)
        self.output_text.setReadOnly(True)
        self.output_text.setStyleSheet('border: 2px solid #4d4d4d; border-radius: 10px; padding: 5px;')

        bg_color = QtGui.QColor(41, 46, 54)
        text_color = QtGui.QColor(255, 255, 255)

        # Set the background color and text color of the widgets
        for input_line in self.input_lines:
            input_line.setStyleSheet(f"background-color: {bg_color.name()}; color: {text_color.name()};")
        self.send_button.setStyleSheet(f"background-color: {bg_color.name()}; color: {text_color.name()};")
        self.output_text.setStyleSheet(f"background-color: {bg_color.name()}; color: {text_color.name()};")

        # Add the widgets to the layout
        layout = QtWidgets.QHBoxLayout()
        for input_line in self.input_lines:
            layout.addWidget(input_line)
        layout.addWidget(self.send_button)

        output_layout = QtWidgets.QVBoxLayout()
        output_layout.addWidget(self.output_text)

        main_layout = QtWidgets.QVBoxLayout()
        main_layout.addLayout(layout)
        main_layout.addLayout(output_layout)

        central_widget = QtWidgets.QWidget()
        central_widget.setLayout(main_layout)
        self.setCentralWidget(central_widget)

        self.import_button = QtWidgets.QPushButton('Import')
        self.import_button.setFont(font)
        self.import_button.setStyleSheet('border: 2px solid #4d4d4d; border-radius: 10px; padding: 5px;')

        # Add the button to the layout
        layout.addWidget(self.import_button)

        # Connect the button to the file explorer dialog
        self.import_button.clicked.connect(self.import_file)

        self.send_button.clicked.connect(self.send_input)

    def send_input(self):
        user_input = '"'
        for input_line in self.input_lines:
            text = input_line.text()
            if not text == '':
                user_input += text + ' '
        user_input = user_input[:-1] + '"'
        process.stdin.write(b"REQUEST SF \"10 07\"")
        process.stdin.flush()
        self.output_text.appendPlainText("Request sent :" + user_input)

    def import_file(self):
        options = QtWidgets.QFileDialog.Options()
        options |= QtWidgets.QFileDialog.DontUseNativeDialog
        filename, _ = QtWidgets.QFileDialog.getOpenFileName(self, "Import File", "",
                                                            "All Files (*);;Text Files (*.txt)", options=options)
        if filename:
            # TODO: process the imported file
            print(f"File imported: {filename}")

    def closeEvent(self, event):
        process.terminate()
        process.wait()
        event.accept()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)

    app.setStyle('Fusion')
    palette = QtGui.QPalette()
    palette.setColor(QtGui.QPalette.Window, QtGui.QColor(41, 46, 54))
    palette.setColor(QtGui.QPalette.WindowText, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.Base, QtGui.QColor(41, 46, 54))
    palette.setColor(QtGui.QPalette.AlternateBase, QtGui.QColor(51, 57, 68))
    palette.setColor(QtGui.QPalette.ToolTipBase, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.ToolTipText, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.Text, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.Button, QtGui.QColor(41, 46, 54))
    palette.setColor(QtGui.QPalette.ButtonText, QtGui.QColor(255, 255, 255))
    palette.setColor(QtGui.QPalette.BrightText, QtGui.QColor(255, 0, 0))
    palette.setColor(QtGui.QPalette.Highlight, QtGui.QColor(255, 140, 0))
    palette.setColor(QtGui.QPalette.HighlightedText, QtGui.QColor(255, 255, 255))
    app.setPalette(palette)

    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
