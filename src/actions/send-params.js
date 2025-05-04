const PRODUCT_ID = 259;
const VENDOR_ID = 11914;

const FILTERS = [{
    usbVendorId : VENDOR_ID,
    usbProductId : PRODUCT_ID
}]

const OFFSETS = {
    "led-mode1":0,
    "led-mode2":1,
    "led-mode3":2,
    "led-mode4":3,
    "led-mode5":4,
    "led-mode6":5,
    "led-mode7":6
};

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

async function send_params(id) {
    const port = await connectToSerial();
    const writer = port.writable.getWriter();

    let message = 0;
    for (const key of Object.keys(OFFSETS)) {
        if (cockpit.getDataLakeVariableInfo(key).type === 'boolean') {
            const numberValue = Number({{ key }});
            message += numberValue * (2 ** OFFSETS[key]);
        }
    }

    const buffer = new ArrayBuffer(4);
    const view = new DataView(buffer);
    view.setUint32(0, message);

    await writer.write(new Uint8Array(buffer));
    writer.releaseLock();
}

send_params();
