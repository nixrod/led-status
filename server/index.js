const express = require('express');
const bodyParser = require("body-parser");
const app = express();

app.use(bodyParser.json());
app.use(express.static('public'));

let ledStatus = {
    led: false,
    virtualLed: false
};

app.get('/status', function (req, res) {
    res.json(ledStatus);
});

app.post('/status', function (req, res) {
    let payload = req.body;

    if ('led' in payload && typeof(payload.led) === "boolean") {
        ledStatus.led = payload.led;
    }

    if ('virtualLed' in payload && typeof(payload.virtualLed) === "boolean") {
        ledStatus.virtualLed = payload.virtualLed;
    }

    res.end("status updated");
});

app.listen(3000, function () {
    console.log('Example app listening on port 3000!');
});