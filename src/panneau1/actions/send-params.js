const PRODUCT_ID = 259;
const VENDOR_ID = 11914;

const FILTERS = [{
    usbVendorId : VENDOR_ID,
    usbProductId : PRODUCT_ID
}]

const ids_to_send = ["led-intensity"];

// Connects to the first Pico on the list or asks user to select one if none is already connected.
// Returns the open port for communication.
async function connectToSerial(){
    const ports = await navigator.serial.getPorts(); //{ filters: FILTERS }
    const isConnected = ports.length > 0;
    console.log(`Pico seen (by send params) : ${isConnected}.`);
    const port = isConnected ? ports[0] : await navigator.serial.requestPort(); //{ filters: FILTERS }

    // If the port is not open, open it
    if (port.writable == null){
        await port.open({ baudRate: 9600 });
        console.log("Port opened");
     }

     return port;
}

// Toggles the data-lake variable test-antoine and switches led ON or OFF accordingly.
async function send_params(id) {
    const port = await connectToSerial();
    const writer = port.writable.getWriter();

    let message = "";
    Object.keys(window.cockpit.getAllDataLakeVariablesInfo()).forEach((id)=>{
        if (ids_to_send.includes(id)){
            const value = window.cockpit.dataLakeVariableData[id];
            message += `${id} ${value}\n`;
        }
    });

    const encoder = new TextEncoder();
    const data = encoder.encode(message);

    await writer.write(data);
    writer.releaseLock();
  }

send_params();
