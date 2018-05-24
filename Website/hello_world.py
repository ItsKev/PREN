from flask import Flask, render_template, request
import json

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def hello_world():
    print(request.method)

    myfile = open('mydata.json').read()
    mydata = json.loads(myfile)
    lastJsonentry = mydata[len(mydata) - 1]
    lastX = lastJsonentry["x"]
    lastZ = lastJsonentry["z"]
    print("The newest coordinates are: \nx: ", lastX, " z: ", lastZ)

    if request.method == 'POST':
        if request.form.get('Start') == 'Start':
            # @ToDo Start method
            print('Start')
        elif request.form.get('Stop') == 'Stop':
            # @ToDO Stop method
            print('Stop')
        else:
            print('Error')

    return render_template('home.html', x=lastX, z=lastZ)

