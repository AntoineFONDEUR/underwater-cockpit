const PRODUCT_ID = 259;
const VENDOR_ID = 11914;

const FILTERS = [{
    usbVendorId : VENDOR_ID,
    usbProductId : PRODUCT_ID
}]

// Disconnects from port
async function disconnectFromSerial(){
    const ports = await navigator.serial.getPorts(); //{ filters: FILTERS }
    const isConnected = ports.length > 0;
    const port = isConnected ? ports[0] : await navigator.serial.requestPort(); //{ filters: FILTERS }

    // If the port is opened, close it
    if (port.writable != null) {
        await port.close();
        console.log("Port closed");
    }
}

disconnectFromSerial();
