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

    xData = []
    zData = []

    for i in range(0, len(mydata)):
        xData.append(mydata[i]["x"])
        zData.append(mydata[i]["z"])

    newData = list(zip(xData, zData))

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
        else:
            print('Error')

    return render_template('layout.html', x=lastX, z=lastZ, x_data=xData, z_data=zData, posData=newData)


def start_pressed():
    print("start_pressed")
    helper.start_moving()


def stop_pressed():
    print("stop_pressed")
    helper.stop_moving()


def initialize_callbacks(start_moving, stop_moving, reset_image_processor):
    helper.start_moving = start_moving
    helper.stop_moving = stop_moving
    helper.reset_image_processor = reset_image_processor


def start_website():
    app.run(host='0.0.0.0')


def reset_image_processor():
    helper.reset_image_processor()


if __name__ == '__main__':
    app.run()
