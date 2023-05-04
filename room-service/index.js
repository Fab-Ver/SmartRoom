const express = require('express');
const { body, validationResult } = require('express-validator');
const mqtt = require('mqtt');
const client  = mqtt.connect('mqtt://broker.mqtt-dashboard.com');
const { SerialPort, ReadlineParser  } = require('serialport');
const parser = new ReadlineParser()

const PORT = 8080;
const lightData = Array();
const rollerBlindData = Array();

/**----------------------------------------------------------------------------------------------------
 * Section used to connect to Arduino UNO trough serial port.
---------------------------------------------------------------------------------------------------- */
const SERIAL_PORT = 'COM7'; //Change with the Arduino UNO port. 

const serial = new SerialPort({
    path : SERIAL_PORT,
    baudRate : 9600,
}, function (error){
    if(error){
        return console.log('Error: ', error.message);
    }
    console.log('Port opened ' + SERIAL_PORT);
})

serial.pipe(parser);

parser.on('data', (data) => {
    try{
        let jsonData = JSON.parse(data);
        if(jsonData.light !== undefined){
            lightData.push({
                light : jsonData.light,
                date : new Date().toISOString(),
            })
        }
        if(jsonData.roller_blind !== undefined){
            rollerBlindData.push({
                roller_blind : jsonData.roller_blind, 
                date : new Date().toISOString(),
            })
        }
    } catch (error) {
        console.log(error);
        console.log(data);
    }
});

function writeError(error){
    if(error){
        return console.log('Error on write: ', error.message);
    }
    console.log('Message written on serial line');
}

//  Use this code to list the available serial port. 
/*SerialPort.list().then (
  ports => ports.forEach(port =>console.log(port.path)),
  err => console.log(err)
);*/

/**---------------------------------------------------------------------------------------------------- */

/**----------------------------------------------------------------------------------------------------
 * Section used to receive messages from ESP32 through MQTT.
---------------------------------------------------------------------------------------------------- */
client.on('connect', () => {
    console.log('Connected to MQTT broker');
    client.subscribe(['light-state'], () => {
      console.log(`Subscribe to topic light-state`);
    })
    client.subscribe(['roller-blind-state'], () => {
        console.log(`Subscribe to topic roller-blind-state`);
    })
})

client.on('message', (topic,payload) => {
    console.log('Received Message:', topic, payload.toString());
    try {
        let message = JSON.parse(payload.toString());
        if(topic === 'light-state'){
            let data = {
                light : message.light, 
                date : new Date().toISOString(),
            };
            lightData.push(data);
        } else if (topic === 'roller-blind-state'){
            let data = {
                roller_blind : message.roller_blind, 
                date : new Date().toISOString(),
            };
            rollerBlindData.push(data);
        }
        serial.write(payload.toString() + "\n", (err) => writeError(err));
    } catch (error) {
        console.log("Error parsing JSON received from MQTT.");
    }
})

/**---------------------------------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------------------------------- 
 * Section used to send and receive data through HTTP to the dashboard
---------------------------------------------------------------------------------------------------- */
const app = express();

app.use(express.json());

app.post("/current_state", (req, res) => {
    let response = { success : {
        light : lightData.length > 0 ? lightData[lightData.length - 1]['light'] : null,
        roller_blind : rollerBlindData.length > 0 ? rollerBlindData[rollerBlindData.length -1 ]['roller_blind'] : null,
    }}
    res.send(response);
});

app.post("/update_light_chart", (req, res) => {
    let response = { success : {
        light_data : lightData,
    }}
    res.send(response);
});

app.post("/update_roller_blind_chart", (req, res) => {
    let response = { success : {
        roller_blind_data : rollerBlindData,
    }}
    res.send(response);
});

app.post("/manage_room_state", body('light').isBoolean(), body('rollerBlind').isInt(), (req, res) => {
    let result = validationResult(req);
    if(result.isEmpty()){
        let light_state = req.body.light;
        lightData.push({
            light : light_state,
            date : new Date().toISOString(),
        })
        serial.write(JSON.stringify({light : light_state}) + "\n", (err) => writeError(err));

        let roller_blind_state = req.body.rollerBlind;
        rollerBlindData.push({
            roller_blind : roller_blind_state, 
            date : new Date().toISOString(),
        })
        serial.write(JSON.stringify({roller_blind : roller_blind_state}) + "\n", (err) => writeError(err))
        return res.json({success : true});
    }

    res.json({error : result.array()})
});

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});

/**---------------------------------------------------------------------------------------------------- */





