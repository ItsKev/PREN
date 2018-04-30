import threading

from WebsiteHandler import WebsiteHandler
from ImageProcessor import ImageProcessor
from FreedomboardConnector import FreedomboardConnector


class Main:
    website_handler = WebsiteHandler()
    freedomboard_connector = FreedomboardConnector()
    image_processor = ImageProcessor()

    def __init__(self) -> None:
        print("Main_init")
        #website_thread = threading.Thread(target=self.website_handler.startup_webiste)
        #website_thread.start()

        #self.website_handler.initialize_callbacks(self.start, self.stop)

        #update_website_thread = threading.Thread(target=self.website_handler.start_updating_coordinates,
        #                                         args=(self.freedomboard_connector.get_values,))
        #update_website_thread.start()

        threading.Thread(target=self.image_processor.start_detecting,
                         args=(self.freedomboard_connector,)).start()
        self.freedomboard_connector.start()

    def start(self):
        print("Main_start")
        self.freedomboard_connector.start()


if __name__ == '__main__':
    Main()
