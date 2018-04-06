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
        website_thread = threading.Thread(target=self.website_handler.startup_webiste)
        website_thread.start()

        self.website_handler.initialize_callbacks(self.start, self.stop)

        update_website_thread = threading.Thread(target=self.website_handler.start_updating_coordinates,
                                                 args=self.freedomboard_connector.get_values)
        update_website_thread.start()

    def start(self):
        print("Main_start")
        self.freedomboard_connector.start()
        threading.Thread(target=self.freedomboard_connector.start_detecting,
                         args=self.image_processor.start_detecting(self.target_found()))

    def target_found(self):
        print("Main_Target_Found")
        self.image_processor.stop_detecting()
        self.freedomboard_connector.target_found()

    def stop(self):
        print("Main_stop")
        self.image_processor.stop_detecting()
        self.freedomboard_connector.stop()


if __name__ == '__main__':
    Main()
