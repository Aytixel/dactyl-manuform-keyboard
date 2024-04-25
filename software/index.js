async function timeout(timeout) {
    return new Promise(resolve => setTimeout(() => resolve(), timeout))
}

const find_key_search_bar = document.getElementById("find_key")
const key_list = document.getElementById("key_list")
const key_layout = getKeyLayout(keylayout_fr_fr)

function getKeyNameFromCode(code) {
    return Object.entries(key_layout).filter(([_, _code]) => _code == code).map(([key]) => key).join(", ")
}

function updateKeyList() {
    const search = new RegExp(find_key_search_bar.value.split(" ").map(v => `(${v})`).join(".*"), "i")
    const found_key = [...new Set(Object.entries(key_layout).filter(([name]) => search.test(name)).map(([_, code]) => code))].map(code => [getKeyNameFromCode(code), code])

    key_list.innerHTML = "";
    key_list.append(...found_key.map(([name, code]) => {
        const element = document.createElement("li")
        const button = document.createElement("button")

        button.textContent = name
        button.dataset.code = code
        element.append(button)

        return element
    }))
}

updateKeyList()

find_key_search_bar.addEventListener("input", updateKeyList)

const get_config_button = document.getElementById("get_config")
const program_button = document.getElementById("program")
const keyboard_preview = [...document.getElementById("keyboard_preview").children].map(e => e.children)

navigator.serial.addEventListener("connect", console.log)
navigator.serial.addEventListener("disconnect", console.log)

async function openPortThen(callback) {
    const filters = [{ usbVendorId: 0x2341, usbProductId: 0x8037 }]
    const port = await navigator.serial.requestPort({ filters })

    await port.open({ baudRate: 115200 })

    const writer = port.writable.getWriter()
    const reader = port.readable.getReader({ mode: "byob" })

    await callback({ port, writer, reader })

    writer.releaseLock()
    reader.releaseLock()

    await port.close()
}

get_config_button.addEventListener("click", () => openPortThen(async ({ writer, reader }) => {
    await writer.write(new Uint8Array([1]))
    await timeout(200);

    const { value } = await reader.read(new Uint8Array(168))
    const buffer = new Uint8Array(value.buffer)

    for (const i in buffer) {
        const fn = Math.round(i / 168)
        const side = Math.round((i % 84) / 84)
        const keyboard_preview_side = keyboard_preview[fn][side]
        const key_preview = document.createElement("button")

        key_preview.dataset.fn = fn
        key_preview.dataset.side = side
        key_preview.dataset.row = Math.floor((i % 42) / 7)
        key_preview.dataset.col = i % 7
        key_preview.dataset.code = buffer[i]
        key_preview.textContent = getKeyNameFromCode(buffer[i])
        keyboard_preview_side.append(key_preview)
    }
}))

program_button.addEventListener("click", () => open_port_then(async ({ }) => {
}))