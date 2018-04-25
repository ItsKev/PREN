import sqlite3

from flask import Flask, render_template, request

app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def hello_world():
    print(request.method)
    if request.method == 'POST':
        if request.form.get('Start') == 'Start':
            # @ToDo Start method
            print('Start')
        elif request.form.get('Stop') == 'Stop':
            # @ToDO Stop method
            print('Stop')
        elif request.form.get('dataview') == 'dataview':
            return render_template('dataView.html')
    else:
        print('Error')

    return render_template('home.html')


if __name__ == '__main__':
    app.run()
