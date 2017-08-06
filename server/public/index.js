let ledState = false;
const ENDPOINT = 'http://localhost:3000/status';

function toggleLedStatus() {
    $('#updateLedState').prop('disabled', true);
    ledState = !ledState;

    $.ajax({
        type: 'POST',
        url: ENDPOINT,
        data: JSON.stringify ({led: ledState}),
        success: function(data) {
            console.log("Updated led state: " + data);
        },
        contentType: "application/json",
        dataType: 'json'
    });
    updateButtonText();
}

function updateButtonText() {
    if (ledState) {
        $('#updateLedState').text('Turn off led');
    } else {
        $('#updateLedState').text('Turn on led');
    }
}

function pollLedStatus() {
    $.get(ENDPOINT, function (data) {
        console.log("Data Loaded: " + JSON.stringify(data));
        $("#ledState").text(data.led);
        $('#virtualLedState').text(data.virtualLed);

        $('#updateLedState').prop('disabled', false);
        ledState = data.led;
        updateButtonText();
    });
}

$('#updateLedState').click(function () {
    toggleLedStatus();
});

setInterval(pollLedStatus, 3000);
pollLedStatus();