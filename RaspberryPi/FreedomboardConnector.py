import serial
import time


class FreedomboardConnector:

    def __init__(self):
        print("FreedomboardConnector__init__ ")
        print("open serial connection")
        self.serial_connection = serial.Serial('/dev/ttyACM0', 38400)

    def start(self):
        print("FreedomboardConnector_start")
        self.serial_connection.write(b'start\n')

    def stop(self):
        print("FreedomboardConnector_stop")
        self.serial_connection.write(b'stop\n')

    def drive_get_velocity(self) -> float:
        self.serial_connection.write(b'ant status\n')
        line = self.serial_connection.readline().decode()
        if line[0] == 's':
            return float(line.split(';')[1])
        return 0

    def drive_get_steps(self) -> int:
        self.serial_connection.write(b'ant status\n')
        line = self.serial_connection.readline().decode()
        if line[0] == 's':
            return int(line.split(';')[1])
        return 0

    def drive_move_steps(self, steps):
        print("drive_move_steps " + str(steps))
        self.serial_connection.write(b'ant n ' + str(steps).encode() + b'\n')
        print(self.serial_connection.readline())

    def drive_faster(self):
        print("drive_faster")
        self.serial_connection.write(b'ant fast\n')

    def drive_slower(self):
        print("drive_slower")
        self.serial_connection.write(b'ant slow\n')

    def drive_stop(self):
        print("drive_stop")
        self.serial_connection.write(b'ant stop\n')

    def load_get_velocity(self) -> float:
        self.serial_connection.write(b'lst status\n')
        line = self.serial_connection.read(7)
        line.split()
        return float(line[2])

    def load_get_steps(self) -> int:
        self.serial_connection.write(b'lst status\n')
        line = self.serial_connection.read(7)
        line.split()
        return int(line[5])

    def load_move_steps(self, steps):
        print("load_move_steps " + steps)
        self.serial_connection.write("lst " + steps)

    def load_faster(self):
        print("load_faster")
        self.serial_connection.write(b'lst fast')

    def load_slower(self):
        print("load_slower")
        self.serial_connection.write(b'lst slow')

    def load_stop(self):
        print("load_stop")
        self.serial_connection.write(b'lst stop')

    def get_values(self) -> (int, int):
        self.serial_connection.write(b'msg last')
        line = self.serial_connection.read(7)
        line.split()
        return int(line[2]), int(line[5])

    def is_finished(self) -> bool:
        self.serial_connection.write(b'msg last')
        line = self.serial_connection.read(7)
        line.split()
        return bool(line[2])

    def start_detecting(self, callback):
        print("FreedomboardConnector_start_detecting")
        self.serial_connection.read(7)
        callback()
        # TODO: Find a better solution

    def target_found(self):
        print("FreedomboardConnector_target_found")
        self.serial_connection.write(b'lst stop')

    def check_if_successful(self, method):
        line = self.serial_connection.readline().decode()
        if "success" not in line:
            method()
