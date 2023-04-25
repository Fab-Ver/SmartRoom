const express = require('express');
const { body, validationResult } = require('express-validator');
const mqtt = require('mqtt');
const client  = mqtt.connect('mqtt://broker.mqtt-dashboard.com');
let serialport = require('serialport');
 
const PORT = 8080;

const app = express();

const lightData = Array();
const rollerBlindData = Array();

app.use(express.json());
//app.use(express.urlencoded({ extended: true }));

app.post("/data", (req, res) => {
    res.json({ message: "Hello from server!" });
});

app.get("/", (req, res) => {
    res.send([lightData, rollerBlindData]);
});

app.post("/manage_room_state",body('light').isBoolean(),body('rollerBlind').isInt(), (req, res) => {
    let result = validationResult(req);
    if(result.isEmpty()){
        //console.log(req);
        let light_state = req.body.light;
        lightData.push({
            light : light_state,
            date : new Date().toISOString(),
        })
        let roller_blind_state = req.body.rollerBlind;
        rollerBlindData.push({
            roller_blind : roller_blind_state, 
            date : new Date().toISOString(),
        })
        console.log(light_state,roller_blind_state);
        return res.json({success : true});
    }

    res.json({error : result.array()})
});

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});

client.on('connect', () => {
    console.log('Connected')
    client.subscribe(['light-state'], () => {
      console.log(`Subscribe to topic light-state`)
    })
    client.subscribe(['roller-blind-state'], () => {
        console.log(`Subscribe to topic roller-blind-state`)
    })
})

client.on('message', (topic,payload) => {
    console.log('Received Message:', topic, payload.toString())
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
})

// list serial ports:
serialport.SerialPort.list().then (
  ports => ports.forEach(port =>console.log(port.path)),
  err => console.log(err)
)

