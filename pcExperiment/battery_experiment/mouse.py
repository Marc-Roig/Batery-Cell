from pynput.mouse import Listener, Controller, Button
from pynput.keyboard import Listener as KbListener
from pynput.keyboard import Key

from pywinauto.findwindows import ElementNotFoundError, ElementAmbiguousError
import pywinauto

from glob import glob
import numpy as np
import logging
import time


class ClickRecorder:

    def __init__(self, file_name):

        self.file_name = file_name
        self.action_list = []
        self.run = False

        self.mouse_listener = Listener(on_click=self.on_click)
        self.kb_listener = KbListener(on_press=self.on_press)

    def store_sequence(self):

        # Generate file sufix to avoid overriding another with the same name
        sufix = ""
        num_files = len(glob(f"{self.file_name}*.npy"))
        if num_files:
            sufix = f"_{num_files}"

        # Store sequence
        np.save(f"{self.file_name}{sufix}.npy", self.action_list)

    def on_click(self, x, y, button, pressed):

        if pressed:
            logging.info(button)
            if button == Button.left:
                self.action_list.append([Button.left, x, y])
            elif button == Button.right:
                self.action_list.append([Button.right, x, y])

    def on_press(self, key):

        if key == Key.esc:
            self.mouse_listener.stop()
            self.kb_listener.stop()
            self.run = False
            self.store_sequence()

    def start_recording(self):

        # Start recording
        self.mouse_listener.start()
        self.kb_listener.start()
        self.run = True

        logging.info(" START CLICKING")
        while self.run:
            pass

        logging.info(" SEQUENCE OF CLICKS STORED")


class ClickExecutor:

    def __init__(self, file_path, time_click_s=1):

        self.action_list = self.get_sequence(file_path)
        self.time_click_s = time_click_s

    @staticmethod
    def get_sequence(file_path):

        return np.load(file_path, allow_pickle=True)

    def execute_seq(self):

        mouse = Controller()
        for button, dx, dy in self.action_list:
            mouse.position = (dx, dy)
            mouse.press(button)
            mouse.release(button)

            time.sleep(self.time_click_s)


class GuiRobot:

    def __init__(self, windows_name, click_seq_path, time_click_s=1):

        self.windows_name = windows_name
        self.clicker = ClickExecutor(file_path=click_seq_path, time_click_s=time_click_s)

    def maximize_window(self, program_path=None):

        app = pywinauto.application.Application()

        if program_path is None:
            # try:
            # https://stackoverflow.com/questions/39794729/pywinauto-unable-to-bring-window-to-foreground
            app.connect(title_re=f".*{self.windows_name}.*")

            # https://github.com/pywinauto/SWAPY/releases
            app_dialog = app.top_window()
            app_dialog.maximize()
            app_dialog.wait('visible', timeout=20)
            app_dialog.set_focus()

    @staticmethod
    def list_opened_windows():

        # https://stackoverflow.com/questions/57566528/how-do-i-enumerate-all-top-level-windows-in-windows-10-using-pywinauto
        from pywinauto import Desktop
        windows = Desktop(backend="uia").windows()
        return [w.window_text() for w in windows]

    def run(self):

        self.maximize_window()
        self.clicker.execute_seq()
