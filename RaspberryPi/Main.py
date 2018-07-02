import threading
import time

from hello_world import *
from ImageProcessor import ImageProcessor
from FreedomboardConnector import FreedomboardConnector


class Main:
    freedomboard_connector = FreedomboardConnector()
    image_processor = ImageProcessor()
    update_thread = True

    def __init__(self) -> None:
        print("Main_init")
        initialize_callbacks(self.start, self.stop, self.reset_image_processor)

        with open('mydata.json', mode='w', encoding='utf-8') as json_file:
            data = []
            entry = {'z': 0, 'x': 65}
            data.append(entry)
            json.dump(data, json_file)

        threading.Thread(target=start_website).start()
        self.image_processor.start_detecting(self.freedomboard_connector)

    def start(self):
        print("Main_start")
        self.freedomboard_connector.start()
        threading.Thread(target=self.get_coordiantes).start()

    def stop(self):
        print("Main_stop")
        self.freedomboard_connector.stop()
        self.update_thread = False

    def get_coordiantes(self):
        time.sleep(0.1)
        self.freedomboard_connector.start_detecting()
        print('start getting coordinates')
        time.sleep(0.2)
        old_x = 0.0
        while True:
            x, z = self.freedomboard_connector.get_values()
            if x == -1 and z == -1:
                with open('mydata.json') as json_file:
                    data = json.load(json_file)

                with open('mydata.json', mode='w', encoding='utf-8') as json_file:
                    entry = {'z': 1.5, 'x': old_x}
                    data.append(entry)
                    json.dump(data, json_file)
                break
            old_x = x
            data = []
            with open('mydata.json') as json_file:
                data = json.load(json_file)

            with open('mydata.json', mode='w', encoding='utf-8') as json_file:
                entry = {'z': z, 'x': x}
                data.append(entry)
                json.dump(data, json_file)
            time.sleep(0.3)

    def reset_image_processor(self):
        print("Reset")
        self.update_thread = False
        threading.Thread(target=self.image_processor.start_detecting, args=(self.freedomboard_connector, )).start()
        threading.Thread(target=self.get_coordiantes).start()
        self.update_thread = True

if __name__ == '__main__':
    Main()
