from flask import Flask, render_template, request, jsonify
import json

app = Flask(__name__)


class Helper:
    start_moving = None
    stop_moving = None
    reset_image_processor = None

    already_started = False


helper = Helper()


@app.route('/', methods=['GET', 'POST'])
def hello_world():
    # print(request.method)

    myfile = open('mydata.json').read()
    mydata = json.loads(myfile)
    lastJsonentry = mydata[len(mydata) - 1]
    lastX = lastJsonentry["x"]
    lastZ = lastJsonentry["z"]
    # print("The newest coordinates are: \nx: ", lastX, " z: ", lastZ)

    if request.method == 'POST':
        if request.form.get('Start') == 'Start':
            if not helper.already_started:
                helper.start_moving()
                print('Start')
                helper.already_started = True
        elif request.form.get('Stop') == 'Stop':
            helper.stop_moving()
            print('Stop')
        elif request.form.get('Reset') == 'Reset':
            if helper.already_started:
                helper.already_started = False
                reset_image_processor()
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

    return render_template('layout.html', x=lastX, z=lastZ)


@app.route('/newPosition')
def writeNewPosition(x, z):
    newX = x
    newZ = z
    # @ToDo


def start_pressed():
    print("start_pressed")
    helper.start_moving()


def stop_pressed():
    print("stop_pressed")
    helper.stop_moving()

    # def start_updating_coordinates(self, get_coordinates):
    #    x, z = get_coordinates()
    #    print("got_ values" + x + z)


def initialize_callbacks(start_moving, stop_moving, reset_image_processor):
    helper.start_moving = start_moving
    helper.stop_moving = stop_moving
    helper.reset_image_processor = reset_image_processor


def start_website():
    app.run(host='0.0.0.0')


def reset_image_processor():
    helper.reset_image_processor()
