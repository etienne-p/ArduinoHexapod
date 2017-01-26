'''
A simple websocket server used to update the bot's settings using a json file
this way we do not have to recompile the Arduino code to tweak the bot's settings
'''
import asyncio
from tkinter import *
import json
import time
import serial
import threading
from cmd import Cmd
from math import atan2, pi
from SimpleWebSocketServer import SimpleWebSocketServer, WebSocket

def lerp(a, b, x):
    return a + x * (b - a);

clients = [] # bot microcontroller

# send a message to the bot(s)
def broadcast(msg):
    for client in clients:
        client.sendMessage(msg)

# convert gait label to int
def gait_label_to_index(label):
    if label == "none":
        return 0
    elif label == "metachronal":
        return 1
    elif label == "ripple":
        return 2
    elif label == "tripod":
        return 3
    else:
        raise NameError("unexpected label name:" + label)

def gait_index_to_label(index):
    if index == 0:
        return "none"
    elif index == 1:
        return "metachronal"
    elif index == 2:
        return "ripple"
    elif index == 3:
        return "tripod"
    else:
        raise IndexError("unexpected index:" + index)

gait_options = ["none", "metachronal", "ripple", "tripod"]

# describe sliders settings
# tuples: (name, default, min, max)
sliders_spec =  [("freq", 1.0, 0.0, 12.0),
                 ("sig", 1.0, 0.0, 6.0),
                 ("smoothing", 1.0, 0.5, 8.0),
                 ("shoulderAngleLimMin", -18.0, -30.0, 0.0),
                 ("shoulderAngleLimMax", 18.0, 0.0, 30.0),
                 ("kneeAngleLimMin", -18.0, -90.0, 0.0),
                 ("kneeAngleLimMax", 18.0, 0.0, 90.0)]

class ControlUI():
    def __init__(self):
        self.update_job = None
        self.root = Tk()
        self.vars_sliders = dict()
        self.sliders_locomotion = dict()
        self.frame_top = Frame(self.root, relief=RAISED, borderwidth=2, padx=20, pady=20)
        self.frame_top.pack(fill=BOTH, side=TOP, expand=True)
        # gait option menu
        self.gait_var = StringVar(self.frame_top)
        self.gait_var.set(gait_options[0])
        gait_option_label = Label(self.frame_top, text="Gait Pattern")
        gait_option_label.pack()
        self.gait_option_menu = OptionMenu(self.frame_top, self.gait_var, *gait_options)
        self.gait_option_menu.pack()
        # sliders 
        self.frame_sliders = Frame(self.frame_top, relief=RAISED, borderwidth=2, padx=20, pady=20)
        self.frame_sliders.pack(fill=BOTH, side=LEFT, expand=True)
        i = 0
        for spec in sliders_spec:
            self.vars_sliders[spec[0]] = DoubleVar(self.frame_sliders)
            self.vars_sliders[spec[0]].set(spec[1])
            self.sliders_locomotion[spec[0]] = Scale(self.frame_sliders, 
                variable=self.vars_sliders[spec[0]], 
                label=spec[0], 
                from_=spec[2], 
                to=spec[3], 
                orient=HORIZONTAL,
                resolution=0.001)
            self.sliders_locomotion[spec[0]].grid(row=int(i%6), column=int(i/6), sticky=N+S+E+W)
            i = i + 1
        # set columns weight to fill the frame
        for i in range(3):
            self.frame_sliders.columnconfigure(i, weight=1)
        # calibration
        self.frame_calibration = Frame(self.frame_top, relief=RAISED, borderwidth=2, padx=20, pady=20)
        self.frame_calibration.pack(fill=BOTH, side=LEFT, expand=True)
        self.sliders_calibration = []
        self.vars_calibration = []
        servo_types = ['shoulder', 'knee', 'neck']
        # setup servo calibration sliders
        for i in range(14):
            self.vars_calibration.append(DoubleVar(self.frame_calibration))
            slider = Scale(self.frame_calibration, 
                variable=self.vars_calibration[i], 
                label=servo_types[int(i/6)] + '_' + str(int(i%6)), 
                from_=-24, 
                to=24, 
                orient=HORIZONTAL,
                resolution=0.001)
            # N+S+E+W -> Expand in all directions = Fill
            slider.grid(row=int(i%6), column=int(i/6), sticky=N+S+E+W) 
            self.sliders_calibration.append(slider)
        # set columns weight to fill the frame
        for i in range(3):
            self.frame_calibration.columnconfigure(i, weight=1)

        self.frame_bottom = Frame(self.root, relief=RAISED, borderwidth=2, padx=20, pady=20)
        self.frame_bottom.pack(fill=BOTH, side=TOP, expand=True)

        self.button_update = Button(self.frame_bottom, text="Update", command=self.update)
        self.button_update.pack(side=LEFT)

        self.button_auto_update = Button(self.frame_bottom, command=self.toggle_auto_validate)
        self.button_auto_update.pack(side=LEFT)
        self.continuous_validation = False
        self.set_auto_update(self.continuous_validation)

        self.button_write_settings = Button(self.frame_bottom, text="Save Settings", command=self.write_settings)
        self.button_write_settings.pack(side=LEFT)

        self.button_load_settings = Button(self.frame_bottom, text="Load Settings", command=self.load_settings)
        self.button_load_settings.pack(side=LEFT)

    def run(self):
        self.root.mainloop()
    
    def update(self):
       print("update")
       broadcast(self.settings_as_json()) # keeping things simple

    def set_auto_update(self, arg):
        if self.update_job is not None:
            self.root.after_cancel(self.update_job)
            self.update_job = None
        self.button_auto_update.config(text="Auto Update " + ("ON" if arg else "OFF"))
        self.continuous_validation = arg
        if self.continuous_validation:
            self.auto_update_coroutine()

    def toggle_auto_validate(self):
        self.set_auto_update(not self.continuous_validation)

    def auto_update_coroutine(self):
        if self.continuous_validation:
            self.update()
            self.update_job = self.root.after(250, self.auto_update_coroutine)

    def load_settings(self):
        with open('settings.json') as data_file:    
            data = json.load(data_file)
            self.gait_var.set(gait_index_to_label(data["gait"]))
            for spec in sliders_spec:
                self.vars_sliders[spec[0]].set(data[spec[0]])
            for i in range(len(data["servoCalibration"])):
                self.vars_calibration[i].set(data["servoCalibration"][i])

    def write_settings(self):
        with open("settings.json", 'w') as settings_file:
            settings_file.write(self.settings_as_json())

    def settings_as_json(self):
        calibration = []
        for v in self.vars_calibration:
            calibration.append(v.get())
        settings = {
            "gait": gait_label_to_index(self.gait_var.get()),
            "servoCalibration": calibration
        }
        for spec in sliders_spec:
            settings[spec[0]] = self.vars_sliders[spec[0]].get()
        return json.dumps(settings)

class SimpleWebSocket(WebSocket):

    def handleMessage(self):
        pass

    def handleConnected(self):
        clients.append(self)
        print(self.address, 'connected')

    def handleClose(self):
        clients.remove(self)
        print(self.address, 'closed')

if __name__ == '__main__':

    socket_thread = threading.Thread(target=SimpleWebSocketServer('', 3000, SimpleWebSocket).serveforever)
    socket_thread.daemon = True
    socket_thread.start()

    ControlUI().run()
