async function timeout(timeout) {
    return new Promise(resolve => setTimeout(() => resolve(), timeout))
}

const LAYER_COUNT = 6
const SIDE_COUNT = 2
const ROW_LEN = 6
const COL_LEN = 7

const SIDE_LEN = ROW_LEN * COL_LEN
const LAYER_LEN = SIDE_LEN * SIDE_COUNT
const ALL_LAYER_LEN = LAYER_LEN * LAYER_COUNT

const METHOD_GET_KEY_LAYOUT = 1
const METHOD_SET_KEY = 2

const find_key_search_bar = document.getElementById("find_key")
const key_list = document.getElementById("key_list")
const key_layout = getKeyLayout(keylayout_fr_fr)
let selected_key = { name: "", code: 0 }

function updateKeyList() {
    const search = new RegExp(find_key_search_bar.value.split(" ").map(v => `(${v})`).join(".*"), "i")
    const found_key = [...new Set(Object.entries(key_layout))].map(([code, names]) => [code, names.join(", ")]).filter(([_, name]) => name.length && search.test(name))

    key_list.innerHTML = "";
    key_list.append(...found_key.map(([code, name]) => {
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
    return [...new Array(LAYER_COUNT)].map(() => [...new Array(SIDE_COUNT)].map(() => [...new Array(ROW_LEN)].map(() => [...new Array(COL_LEN)])))
}

const get_config_button = document.getElementById("get_config")
const program_button = document.getElementById("program")
const download_button = document.getElementById("download")
const upload_file = document.getElementById("upload")
const keyboard_preview = document.getElementById("keyboard_preview")
const keyboard_preview_children = []
const old_key_layout = generateConfigKeyLayout()
const new_key_layout = generateConfigKeyLayout()
const key_preview_layout = generateConfigKeyLayout()

for (let i = 0; i < LAYER_COUNT; i++) {
    const state = document.createElement("div")
    const left = document.createElement("div")
    const right = document.createElement("div")

    state.classList.add("state")
    left.classList.add("left")
    right.classList.add("right")
    state.append(left, right)
    keyboard_preview.append(state)
    keyboard_preview_children.push(state.children)
}

document.getElementById("show_layer_state").addEventListener("input", () => keyboard_preview.classList.toggle("layer_state"))

function indexToIndexes(i) {
    return {
        layer: Math.floor(i / LAYER_LEN),
        side: Math.floor((i % LAYER_LEN) / SIDE_LEN),
        row: Math.floor((i % SIDE_LEN) / COL_LEN),
        col: i % COL_LEN
    }
}

for (let i = 0; i < ALL_LAYER_LEN; i++) {
    const { layer, side, row, col } = indexToIndexes(i)
    const key_preview = document.createElement("button")

    function setConfigKey(layer, side, row, col, name, code) {
        key_preview.textContent = name
        key_preview.dataset.code = code
        new_key_layout[layer][side][row][col] = code
    }

    key_preview.addEventListener("dblclick", () => setConfigKey(layer, side, row, col, "", 0))
    key_preview.addEventListener("click", () => setConfigKey(layer, side, row, col, selected_key.name, selected_key.code))

    key_preview.textContent = ""
    key_preview.dataset.code = 0
    key_preview.dataset.layer = layer
    key_preview.dataset.side = side
    key_preview.dataset.row = row
    key_preview.dataset.col = col

    keyboard_preview_children[layer][side].append(key_preview_layout[layer][side][row][col] = key_preview)
}

function updateKeyboardPreview() {
    for (let i = 0; i < ALL_LAYER_LEN; i++) {
        const { layer, side, row, col } = indexToIndexes(i)

        key_preview_layout[layer][side][row][col].dataset.code = new_key_layout[layer][side][row][col]
        key_preview_layout[layer][side][row][col].textContent = key_layout[new_key_layout[layer][side][row][col]].join(", ")
    }
}

get_config_button.addEventListener("click", () => openPortThen(async ({ writer, reader }) => {
    await writer.write(new Uint8Array([METHOD_GET_KEY_LAYOUT]))
    await timeout(200)

    buffer = new Uint8Array((await reader.read(new Uint8Array(ALL_LAYER_LEN))).value.buffer)

    for (const i in buffer) {
        const { layer, side, row, col } = indexToIndexes(i)

        old_key_layout[layer][side][row][col] = buffer[i]
        new_key_layout[layer][side][row][col] = buffer[i]
    }

    updateKeyboardPreview()
}))

program_button.addEventListener("click", () => openPortThen(async ({ writer }) => {
    for (let i = 0; i < ALL_LAYER_LEN; i++) {
        const { layer, side, row, col } = indexToIndexes(i)

        if (old_key_layout[layer][side][row][col] != new_key_layout[layer][side][row][col]) {
            await writer.write(new Uint8Array([
                METHOD_SET_KEY,
                (layer & 0b00000111)
                ((side & 0b00000001) << 6) + ((row & 0b00000111) << 3) + (col & 0b00000111),
                new_key_layout[layer][side][row][col] & 0xFF00 >> 8,
                new_key_layout[layer][side][row][col] & 0x00FF,
            ]))

            old_key_layout[layer][side][row][col] = new_key_layout[layer][side][row][col]
        }

        program_button.children[0].value = i + 1
    }

    await timeout(500)

    program_button.children[0].value = 0
}))

download_button.addEventListener("click", () => {
    const link = document.createElement("a")

    link.setAttribute("href", "data:application/json;charset=utf-8," + encodeURIComponent(JSON.stringify(new_key_layout)))
    link.setAttribute("download", "keyboard-config.json")

    link.style.display = "none"

    document.body.appendChild(link)

    link.click()

    document.body.removeChild(link)
})

upload_file.addEventListener("change", async () => {
    const loaded_key_layout = JSON.parse(await upload_file.files[0].text())

    for (let i = 0; i < ALL_LAYER_LEN; i++) {
        const { layer, side, row, col } = indexToIndexes(i)

        new_key_layout[layer][side][row][col] = loaded_key_layout[layer][side][row][col]
    }

    updateKeyboardPreview()
})