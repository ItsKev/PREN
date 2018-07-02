import serial
import threading
import time

class FreedomboardConnector:

    lock = threading.Lock()

    def __init__(self):
        print("FreedomboardConnector__init__ ")
        print("open serial connection")
        self.lock.acquire()
        self.serial_connection = serial.Serial('/dev/ttyACM0', 38400)
        self.lock.release()

    def start(self):
        print("FreedomboardConnector_start")
        self.lock.acquire()
        print("start lock aquired!")
        while True:
            self.serial_connection.write(b'start\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def stop(self):
        print("FreedomboardConnector_stop")
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'stop\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def drive_get_velocity(self) -> float:
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'ant status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                self.lock.release()
                return float((line.split(' ')[1]).split(';')[1])

    def drive_get_steps(self) -> int:
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'ant status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                self.lock.release()
                return int((line.split(' ')[1]).split(';')[0])

    def drive_move_steps(self, steps):
        print("drive_move_steps " + str(steps))
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'ant n ' + str(steps).encode() + b'\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def drive_faster(self):
        print("drive_faster")
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'ant fast\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def drive_slower(self):
        print("drive_slower")
        self.lock.acquire()
        self.serial_connection.write(b'ant slow\n')

    def drive_stop(self):
        print("drive_stop")
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'ant stop\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def load_get_velocity(self) -> float:
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'lst status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                self.lock.release()
                return float((line.split(' ')[1]).split(';')[1])

    def load_get_steps(self) -> int:
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'lst status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                self.lock.release()
                return int((line.split(' ')[1]).split(';')[0])

    def load_move_steps(self, steps):
        print("load_move_steps " + steps)
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'lst n ' + str(steps).encode() + b'\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def load_faster(self):
        print("load_faster")
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'lst fast\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def load_slower(self):
        print("load_slower")
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'lst slow\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def load_stop(self):
        print("load_stop")
        self.lock.acquire()
        while True:
            self.serial_connection.write(b'lst stop\n')
            if self.check_if_successful():
                break
        self.lock.release()

    def get_values(self) -> (float, float):
        while True:
            self.lock.acquire()
            self.serial_connection.write(b'msg last\n')
            read_line = self.serial_connection.readline().decode()
            self.lock.release()
            if "s " in read_line:
                read_line = read_line.split(' ')[1]
                print(read_line)
                return float(read_line.split(';')[0]), float(read_line.split(';')[1])
            elif "measurement finished" in read_line:
                print("measurement finished")
                return -1.0, -1.0
            time.sleep(0.1)
            read_line = ""

    def start_detecting(self):
        print("FreedomboardConnector_start_detecting")
        while True:
            self.lock.acquire()
            line = self.serial_connection.readline().decode()
            self.lock.release()
            if "start detecting" in line:
                break

    def target_found(self):
        print("FreedomboardConnector_target_found")
        self.lock.acquire()
        print("target found lock acquired")
        while True:
            self.serial_connection.write(b'target found\n')
            if self.check_if_successful():
                break
        time.sleep(0.2)
        self.lock.release()

    def check_if_successful(self) -> bool:
        line = self.serial_connection.readline().decode()
        if "success" not in line:
            return False
        return True
