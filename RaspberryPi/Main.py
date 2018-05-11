import threading

from hello_world import *
from ImageProcessor import ImageProcessor
from FreedomboardConnector import FreedomboardConnector


class Main:
    freedomboard_connector = FreedomboardConnector()
    image_processor = ImageProcessor()

    def __init__(self) -> None:
        print("Main_init")
        initialize_callbacks(self.start, self.stop)
        threading.Thread(target=app.run).start()

        #update_website_thread = threading.Thread(target=self.website_handler.start_updating_coordinates,
        #                                         args=(self.freedomboard_connector.get_values,))
        #update_website_thread.start()

        self.image_processor.start_detecting(self.freedomboard_connector)

    def start(self):
        print("Main_start")
        self.freedomboard_connector.start()

    def stop(self):
        print("Main_stop")
        self.freedomboard_connector.stop()


if __name__ == '__main__':
    Main()
