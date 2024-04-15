"use strict";

// Client-side interactions with the browser.
//chatgpt used for optimization of militry time conversion
// Web sockets: automatically establish a socket with the server
var socket = io.connect();

$(document).ready(function() {
    // Make connection to server when web page is fully loaded.

    $('#toggleSwitch').change(function() {
        // Check if the switch is checked (on) or not (off)
        if (this.checked) {
            // Do something when switch is on
            console.log("Switch is ON");
            sendCommandViaUDP("on");
        } else {
            // Do something when switch is off
            console.log("Switch is OFF");
            sendCommandViaUDP("off");
        }
    });

    $('#send-form').submit(function(event) {
        // Prevent default form submission behavior
        event.preventDefault();

        // Get the value of the input field
        var inputText = $('#send-command').val();

        sendCommandViaUDP("UPDATE:" + inputText);
    });

	$('#stop').click(function(){
		sendCommandViaUDP("EXIT:")
	});

    // Listen for form submission on the schedule submit button
	$('#sch-sub-button').click(function(event) {
		// Prevent default button click behavior
		event.preventDefault();
	
		var scheduleString = "SCHED:";
	
		['monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday'].forEach(function(day) {
	
			// If the day is closed, XX
			// If the day is open, start and close time
			var startTime = $(`input[name="${day}Open"]`).val();
			var closeTime = $(`input[name="${day}Close"]`).val();
	
			startTime = formatTime(startTime);
			closeTime = formatTime(closeTime);
			scheduleString += `${day.charAt(0).toUpperCase()}-${startTime}-${closeTime}`;
	
			scheduleString += `,`;
		});
	
		sendCommandViaUDP(scheduleString);
	});
	
});

function sendCommandViaUDP(message) {
    socket.emit('daUdpCommand', message);
};

function formatTime(time) {
    if (!time) return 'XX:XX';

    const [hours, minutes] = time.split(':');
    let hour = parseInt(hours);
    const minute = parseInt(minutes);

    if (hour === 12 && time.endsWith('AM')) {
        // Midnight (12:XXAM) should be converted to 00:XX
        hour = 0;
    } else if (hour !== 12 && time.endsWith('PM')) {
        // Convert PM times (except 12PM) to 24-hour format
        hour += 12;
    }

    // Format hour and minute with leading zeros
    const formattedHour = hour.toString().padStart(2, '0');
    const formattedMinute = minute.toString().padStart(2, '0');

    return `${formattedHour}:${formattedMinute}`;
}
