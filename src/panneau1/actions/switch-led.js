const PRODUCT_ID = 259;
const VENDOR_ID = 11914;

const FILTERS = [{
    usbVendorId : VENDOR_ID,
    usbProductId : PRODUCT_ID
}]

// Connects to the first Pico on the list or asks user to select one if none is already connected.
// Returns the open port for communication.
async function connectToSerial(){
    const ports = await navigator.serial.getPorts(); // { filters: FILTERS }
    const isConnected = ports.length > 0;
    const port = isConnected ? ports[0] : await navigator.serial.requestPort(); // { filters: FILTERS }

    // If the port is not open, open it
    if (port.writable == null){
        await port.open({ baudRate: 9600 });
        console.log("Port opened");
     }
     console.log("Connected successfully to port.")
     return port;
}

// Toggles the data-lake variable test-antoine and switches led ON or OFF accordingly.
async function switch_led(){
    const port = await connectToSerial();
    const writer = port.writable.getWriter();
    let data;
    if (window.cockpit.dataLakeVariableData["test-antoine"]){
        console.log("Turning led OFF");
        data = new Uint8Array([108]); //108 -> l (for low)
        window.cockpit.setDataLakeVariableData('test-antoine',false);
    }
    else{
        console.log("Turning led ON");
        data = new Uint8Array([104]); //104 -> h (for high)
        window.cockpit.setDataLakeVariableData('test-antoine',true);
    }
    await writer.write(data);
    writer.releaseLock();
}

console.log("Switching led");
switch_led();
