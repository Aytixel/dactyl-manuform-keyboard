async function timeout(timeout) {
    return new Promise(resolve => setTimeout(() => resolve(), timeout))
}

const find_key_search_bar = document.getElementById("find_key")
const key_list = document.getElementById("key_list")
const key_layout = getKeyLayout(keylayout_fr_fr)
let selected_key = { name: "", code: 0 }

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

        button.addEventListener("click", () => selected_key = { name, code })

        button.textContent = name
        button.dataset.code = code
        element.append(button)

        return element
    }))
}

updateKeyList()

find_key_search_bar.addEventListener("input", updateKeyList)

async function openPortThen(callback) {
    const filters = [{ usbVendorId: 0x2341, usbProductId: 0x8036 }]
    const port = await navigator.serial.requestPort({ filters })

    await port.open({ baudRate: 115200 })

    const writer = port.writable.getWriter()
    const reader = port.readable.getReader({ mode: "byob" })

    await callback({ port, writer, reader })

    writer.releaseLock()
    reader.releaseLock()

    await port.close()
}

function generateConfigKeyLayout() {
    return [...new Array(2)].map(() => [...new Array(2)].map(() => [...new Array(6)].map(() => [...new Array(7)])))
}

const get_config_button = document.getElementById("get_config")
const program_button = document.getElementById("program")
const keyboard_preview = document.getElementById("keyboard_preview")
const keyboard_preview_children = [...keyboard_preview.children].map(e => e.children)
const old_key_layout = generateConfigKeyLayout()
const new_key_layout = generateConfigKeyLayout()
const key_preview_layout = generateConfigKeyLayout()

document.getElementById("show_fn_state").addEventListener("input", () => keyboard_preview.classList.toggle("fn_state"))

function indexToIndexes(i) {
    return {
        fn: Math.round(i / 168),
        side: Math.round((i % 84) / 84),
        row: Math.floor((i % 42) / 7),
        col: i % 7
    }
}

get_config_button.addEventListener("click", () => openPortThen(async ({ writer, reader }) => {
    await writer.write(new Uint8Array([3]))

    let buffer = new Uint8Array((await reader.read(new Uint8Array(1))).value.buffer)

    const fn_side = (buffer[0] & 0b01000000) >> 6;
    const fn_row = (buffer[0] & 0b00111000) >> 3;
    const fn_col = buffer[0] & 0b00000111;

    await writer.write(new Uint8Array([1]))
    await timeout(200)

    buffer = new Uint8Array((await reader.read(new Uint8Array(168))).value.buffer)

    for (let fn = 0; fn < 2; fn++)
        for (let side = 0; side < 2; side++)
            keyboard_preview_children[fn][side].innerHTML = ""

    for (const i in buffer) {
        const { fn, side, row, col } = indexToIndexes(i)
        const keyboard_preview_side = keyboard_preview_children[fn][side]
        const key_preview = document.createElement("button")

        function setConfigKey(fn, side, row, col, name, code) {
            if (code == FN_KEY) {
                key_preview_layout[1 - fn][side][row][col].textContent = name
                key_preview_layout[1 - fn][side][row][col].dataset.code = code
                new_key_layout[1 - fn][side][row][col] = code
            }
            else if (new_key_layout[fn][side][row][col] == FN_KEY) {
                key_preview_layout[1 - fn][side][row][col].textContent = ""
                key_preview_layout[1 - fn][side][row][col].dataset.code = 0
                new_key_layout[1 - fn][side][row][col] = 0
            }

            key_preview.textContent = name
            key_preview.dataset.code = code
            new_key_layout[fn][side][row][col] = code
        }

        key_preview.addEventListener("dblclick", () => setConfigKey(fn, side, row, col, "", 0))
        key_preview.addEventListener("click", () => setConfigKey(fn, side, row, col, selected_key.name, selected_key.code))

        key_preview.dataset.fn = fn
        key_preview.dataset.side = side
        key_preview.dataset.row = row
        key_preview.dataset.col = col

        if ((side == fn_side && row == fn_row && col == fn_col) || new_key_layout[fn][side][row][col] == FN_KEY) {
            old_key_layout[0][side][row][col] = FN_KEY
            old_key_layout[1][side][row][col] = FN_KEY
            new_key_layout[0][side][row][col] = FN_KEY
            new_key_layout[1][side][row][col] = FN_KEY

            if (key_preview_layout[1 - fn][side][row][col]) {
                key_preview_layout[1 - fn][side][row][col].dataset.code = FN_KEY
                key_preview_layout[1 - fn][side][row][col].textContent = "Fn"
            }

            key_preview.dataset.code = FN_KEY
            key_preview.textContent = "Fn"
        } else {
            old_key_layout[fn][side][row][col] = buffer[i]
            new_key_layout[fn][side][row][col] = buffer[i]
            key_preview.dataset.code = buffer[i]
            key_preview.textContent = getKeyNameFromCode(buffer[i])
        }

        key_preview_layout[fn][side][row][col] = key_preview
        keyboard_preview_side.append(key_preview)
    }
}))

program_button.addEventListener("click", () => openPortThen(async ({ writer }) => {
    for (let i = 0; i < 168; i++) {
        const { fn, side, row, col } = indexToIndexes(i)

        if (old_key_layout[fn][side][row][col] != new_key_layout[fn][side][row][col]) {
            if (new_key_layout[fn][side][row][col] == FN_KEY)
                await writer.write(new Uint8Array([4, ((side & 0b00000001) << 6) + ((row & 0b00000111) << 3) + (col & 0b00000111)]))
            else
                await writer.write(new Uint8Array([2, ((fn & 0b00000001) << 7) + ((side & 0b00000001) << 6) + ((row & 0b00000111) << 3) + (col & 0b00000111), code]))

            old_key_layout[fn][side][row][col] = new_key_layout[fn][side][row][col]
        }

        program_button.children[0].value = i + 1
    }

    await timeout(500)

    program_button.children[0].value = 0
}))