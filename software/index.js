async function timeout(timeout) {
    return new Promise(resolve => setTimeout(() => resolve(), timeout))
}

const get_config_button = document.getElementById("get_config")
const program_button = document.getElementById("program")

navigator.serial.addEventListener("connect", console.log)
navigator.serial.addEventListener("disconnect", console.log)

async function open_port_then(callback) {
    const usbVendorId = 0x2341
    const usbProductId = 0x8036
    const filters = [{ usbVendorId, usbProductId }]
    const port = await navigator.serial.requestPort({ filters })

    await port.open({ baudRate: 115200 })

    const writer = port.writable.getWriter()
    const reader = port.readable.getReader({ mode: "byob" })

    await callback({ port, writer, reader })

    writer.releaseLock()
    reader.releaseLock()

    await port.close()
}

get_config_button.addEventListener("click", () => open_port_then(async ({ writer, reader }) => {
    await writer.write(new Uint8Array([1]))

    const { value, done } = await reader.read(new Int8Array(168))

    console.log(new Int8Array(value.buffer))
}))

program_button.addEventListener("click", () => open_port_then(async ({ }) => {
}))