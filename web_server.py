"""EE 250L Lab 11 Final Project

web_server.py: Event logging server.

TODO: List team members here.
Adit Bhartia
Mia Ahmady

TODO: Insert Github repository link here.
https://github.com/usc-ee250-fall2019/finalproj-lab11-aditmia/tree/finalproj
"""

from flask import Flask
from flask import jsonify
from flask import request

app = Flask('Final Project Web Server')

#for receiving updates from sensor_processing

@app.route('/event', methods=['POST'])
def post_event_callback():
    payload = request.get_json()
    
    #format string to be shown
    formattedString = str(payload['time']) + ": " + str(payload['event']) + " at " + str(payload['room']) 
    
    events.append(formattedString)
    print(formattedString)
    #success message to be sent back to sensor_processing
    return {"Status: ": "Event received"}

#callback for accessing history on web browser

@app.route('/history', methods=['GET'])
def get_history_callback():
    print(events)
    #print each event on a new line
    return "<br>".join(events)

if __name__ == '__main__':
    
    #stores all of the events in a given run
    events = []
    app.run(debug=False, host='127.0.0.1', port=5000)

