from flask import Flask, render_template, request, jsonify
import json

app = Flask(__name__)

start_moving = None
stop_moving = None


@app.route('/', methods=['GET', 'POST'])
def hello_world():
    #print(request.method)

    myfile = open('mydata.json').read()
    mydata = json.loads(myfile)
    lastJsonentry = mydata[len(mydata) - 1]
    lastX = lastJsonentry["x"]
    lastZ = lastJsonentry["z"]
    #print("The newest coordinates are: \nx: ", lastX, " z: ", lastZ)

    if request.method == 'POST':
        if request.form.get('Start') == 'Start':
            hello_world.start_moving()
            print('Start')
        elif request.form.get('Stop') == 'Stop':
            hello_world.stop_moving()
            print('Stop')
        # elif request.form.get('Position') == 'Position':
        #     # @ToDo get Position
        #     myfile = open('mydata.json').read()
        #     mydata = json.loads(myfile)
        #     lastJsonentry = mydata[len(mydata) - 1]
        #     lastX = lastJsonentry["x"]
        #     lastZ = lastJsonentry["z"]
        #     print("The newest coordinates are: \nx: ", lastX, " z: ", lastZ)

        # x, z = WebsiteHandler.WebsiteHandler.start_updating_coordinates()
        else:
            print('Error')

    return render_template('home.html', x=lastX, z=lastZ)


@app.route('/newPosition')
def writeNewPosition(x, z):
    newX = x
    newZ = z
    # @ToDo


def start_pressed():
    print("start_pressed")
    hello_world.start_moving()


def stop_pressed():
    print("stop_pressed")
    hello_world.stop_moving()

    # def start_updating_coordinates(self, get_coordinates):
    #    x, z = get_coordinates()
    #    print("got_ values" + x + z)


def initialize_callbacks(start_moving, stop_moving):
    hello_world.start_moving = start_moving
    hello_world.stop_moving = stop_moving
