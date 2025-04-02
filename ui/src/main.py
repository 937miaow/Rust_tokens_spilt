import sys
import os
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                           QHBoxLayout, QTextEdit, QPushButton, QFileDialog, 
                           QLabel, QMessageBox)
from PyQt5.QtCore import Qt
import subprocess

def get_resource_path(relative_path):
    """获取资源文件的绝对路径"""
    try:
        # PyInstaller创建临时文件夹，将路径存储在_MEIPASS中
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Rust词法分析器")
        self.setGeometry(100, 100, 800, 600)
        
        # 创建主窗口部件
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        
        # 创建垂直布局
        layout = QVBoxLayout()
        main_widget.setLayout(layout)
        
        # 创建输入区域
        input_layout = QHBoxLayout()
        
        # 输入框
        self.input_text = QTextEdit()
        self.input_text.setPlaceholderText("在此输入Rust代码...")
        self.input_text.setMinimumHeight(100)
        input_layout.addWidget(self.input_text)
        
        # 按钮区域
        button_layout = QVBoxLayout()
        
        # 选择文件按钮
        self.select_file_btn = QPushButton("选择文件")
        self.select_file_btn.clicked.connect(self.select_file)
        button_layout.addWidget(self.select_file_btn)
        
        # 分析按钮
        self.analyze_btn = QPushButton("分析")
        self.analyze_btn.clicked.connect(self.analyze_code)
        button_layout.addWidget(self.analyze_btn)
        
        input_layout.addLayout(button_layout)
        layout.addLayout(input_layout)
        
        # 输出区域
        output_label = QLabel("分析结果：")
        layout.addWidget(output_label)
        
        self.output_text = QTextEdit()
        self.output_text.setReadOnly(True)
        self.output_text.setMinimumHeight(300)
        layout.addWidget(self.output_text)
        
    def select_file(self):
        file_name, _ = QFileDialog.getOpenFileName(
            self,
            "选择Rust源文件",
            "",
            "Rust Files (*.rs);;All Files (*.*)"
        )
        if file_name:
            try:
                with open(file_name, 'r', encoding='utf-8') as file:
                    content = file.read()
                    self.input_text.setText(content)
            except Exception as e:
                QMessageBox.critical(self, "错误", f"无法读取文件：{str(e)}")
    
    def analyze_code(self):
        code = self.input_text.toPlainText()
        if not code.strip():
            QMessageBox.warning(self, "警告", "请输入要分析的代码")
            return
            
        try:
            # 将代码写入临时文件
            with open("temp_code.txt", "w", encoding="utf-8") as f:
                f.write(code)
            
            # 获取可执行文件的路径
            exe_path = get_resource_path("Rust_tokens_spilt.exe")
            
            # 调用C++程序进行分析
            result = subprocess.run(
                [exe_path, "temp_code.txt"],
                capture_output=True,
                text=True,
                encoding="utf-8"
            )
            
            # 显示结果
            if result.returncode == 0:
                self.output_text.setText(result.stdout)
            else:
                QMessageBox.critical(self, "错误", f"分析失败：{result.stderr}")
                
        except Exception as e:
            QMessageBox.critical(self, "错误", f"发生错误：{str(e)}")
        finally:
            # 清理临时文件
            if os.path.exists("temp_code.txt"):
                os.remove("temp_code.txt")

def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main() 