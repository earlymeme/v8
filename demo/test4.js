// inspector-demo.js
// Receiver function called by d8.
function receive(message) {
    print(message)
}

const msg = JSON.stringify({
    id: 0,
    method: "Debugger.enable",
});

// Call the function provided by d8.
send(msg);