import threading
import json
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
        threading.Thread(target=start_website).start()

        threading.Thread(target=self.freedomboard_connector.start_detecting,
                         args=(self.get_coordiantes, )).start()
        self.image_processor.start_detecting(self.freedomboard_connector)

    def start(self):
        print("Main_start")
        self.freedomboard_connector.start()

    def stop(self):
        print("Main_stop")
        self.freedomboard_connector.stop()
        self.update_thread = False

    def get_coordiantes(self):
        while True:
            print('start getting coordinates')
            #if not self.update_thread:
            #    break
            x, z = self.freedomboard_connector.get_values()
            print(str(x))
            print(str(z))
            data = []
            with open('mydata.json') as json_file:
                data = json.load(json_file)

            with open('mydata.json', mode='w', encoding='utf-8') as json_file:
                entry = {'z': x, 'x': z}
                data.append(entry)
                json.dump(data, json_file)
            time.sleep(0.5)

    def reset_image_processor(self):
        print("Reset")
        self.update_thread = False
        threading.Thread(target=self.image_processor.start_detecting, args=(self.freedomboard_connector, )).start()
        #threading.Thread(target=self.freedomboard_connector.start_detecting,
        #                 args=(self.get_coordiantes,)).start()
        self.update_thread = True

if __name__ == '__main__':
    Main()
