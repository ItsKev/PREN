class WebsiteHandler:

    start_moving = None
    stop_moving = None

    def __init__(self):
        print("__init__ Website")

    def startup_webiste(self):
        print("Website started")

    def start_pressed(self):
        print("start_pressed")
        self.start_moving()

    def stop_pressed(self):
        print("stop_pressed")
        self.stop_moving()

    def start_updating_coordinates(self, get_coordinates):
        x, z = get_coordinates()
        print("got_ values" + x + z)

    def initialize_callbacks(self, start_moving, stop_moving):
        self.start_moving = start_moving
        self.stop_moving = stop_moving
