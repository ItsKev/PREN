import serial


class FreedomboardConnector:

    def __init__(self):
        print("FreedomboardConnector__init__ ")
        print("open serial connection")
        self.serial_connection = serial.Serial('/dev/ttyACM0', 38400)

    def start(self):
        print("FreedomboardConnector_start")
        while True:
            self.serial_connection.write(b'start\n')
            if self.check_if_successful():
                break

    def stop(self):
        print("FreedomboardConnector_stop")
        while True:
            self.serial_connection.write(b'stop\n')
            if self.check_if_successful():
                break

    def drive_get_velocity(self) -> float:
        while True:
            self.serial_connection.write(b'ant status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                return float((line.split(' ')[1]).split(';')[1])

    def drive_get_steps(self) -> int:
        while True:
            self.serial_connection.write(b'ant status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                return int((line.split(' ')[1]).split(';')[0])

    def drive_move_steps(self, steps):
        print("drive_move_steps " + str(steps))
        while True:
            self.serial_connection.write(b'ant n ' + str(steps).encode() + b'\n')
            if self.check_if_successful():
                break

    def drive_faster(self):
        print("drive_faster")
        while True:
            self.serial_connection.write(b'ant fast\n')
            if self.check_if_successful():
                break

    def drive_slower(self):
        print("drive_slower")
        self.serial_connection.write(b'ant slow\n')

    def drive_stop(self):
        print("drive_stop")
        while True:
            self.serial_connection.write(b'ant stop\n')
            if self.check_if_successful():
                break

    def load_get_velocity(self) -> float:
        while True:
            self.serial_connection.write(b'lst status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                return float((line.split(' ')[1]).split(';')[1])

    def load_get_steps(self) -> int:
        while True:
            self.serial_connection.write(b'lst status\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                return int((line.split(' ')[1]).split(';')[0])

    def load_move_steps(self, steps):
        print("load_move_steps " + steps)
        while True:
            self.serial_connection.write(b'lst n ' + str(steps).encode() + b'\n')
            if self.check_if_successful():
                break

    def load_faster(self):
        print("load_faster")
        while True:
            self.serial_connection.write(b'lst fast\n')
            if self.check_if_successful():
                break

    def load_slower(self):
        print("load_slower")
        while True:
            self.serial_connection.write(b'lst slow\n')
            if self.check_if_successful():
                break

    def load_stop(self):
        print("load_stop")
        while True:
            self.serial_connection.write(b'lst stop\n')
            if self.check_if_successful():
                break

    def get_values(self) -> (float, float):
        while True:
            self.serial_connection.write(b'msg last\n')
            line = self.serial_connection.readline().decode()
            if line[0] == 's':
                line = line.split(' ')[1]
                return float(line.split(';')[0]), float(line.split(';')[1])

    def start_detecting(self):
        print("FreedomboardConnector_start_detecting")
        while True:
            line = self.serial_connection.readline().decode()
            print(line)
            if "start detecting" in line:
                break

    def target_found(self):
        print("FreedomboardConnector_target_found")
        while True:
            self.serial_connection.write(b'target found\n')
            if self.check_if_successful():
                break

    def check_if_successful(self) -> bool:
        line = self.serial_connection.readline().decode()
        if "success" not in line:
            return False
        return True
