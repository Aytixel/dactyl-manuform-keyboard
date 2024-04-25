function getKeyLayout(local_keylayout) {
    const keylayout = {
        Backspace: 0xB2,
        Tab: 0xB3,
        Enter: 0xB0,
        ShiftLeft: 0x81,
        ShiftRight: 0x85,
        ControlLeft: 0x80,
        ControlRight: 0x84,
        AltLeft: 0x82,
        AltRight: 0x86,
        Pause: 0xD0,
        CapsLock: 0xC1,
        Escape: 0xB1,
        Space: 32,
        PageUp: 0xD3,
        PageDown: 0xD6,
        End: 0xD5,
        Home: 0xD2,
        ArrowLeft: 0xD8,
        ArrowUp: 0xDA,
        ArrowRight: 0xD7,
        ArrowDown: 0xD9,
        PrintScreen: 0xCE,
        Insert: 0xD1,
        Delete: 0xD4,
        MetaLeft: 0x83,
        MetaRight: 0x87,
        Menu: 0xED,
        Numpad0: 0xE1,
        Numpad1: 0xE2,
        Numpad2: 0xE3,
        Numpad3: 0xE4,
        Numpad4: 0xE5,
        Numpad5: 0xE6,
        Numpad6: 0xE7,
        Numpad7: 0xE8,
        Numpad8: 0xE9,
        Numpad9: 0xEA,
        NumpadMultiply: 0xDD,
        NumpadAdd: 0xDF,
        NumpadSubtract: 0xDE,
        NumpadDecimal: 0xEB,
        NumpadDivide: 0xDC,
        NumpadEnter: 0xE0,
        F1: 0xC2,
        F2: 0xC3,
        F3: 0xC4,
        F4: 0xC5,
        F5: 0xC6,
        F6: 0xC7,
        F7: 0xC8,
        F8: 0xC9,
        F9: 0xCA,
        F10: 0xCB,
        F11: 0xCC,
        F12: 0xCD,
        F13: 0xF0,
        F14: 0xF1,
        F15: 0xF2,
        F16: 0xF3,
        F17: 0xF4,
        F18: 0xF5,
        F19: 0xF6,
        F20: 0xF7,
        F21: 0xF8,
        F22: 0xF9,
        F23: 0xFA,
        F24: 0xFB,
        NumLock: 0xDB,
        ScrollLock: 0xCF,
        Semicolon: 59,
        Equal: 61,
        Comma: 44,
        Minus: 45,
        Period: 46,
        Slash: 47,
        Backquote: 96,
        BracketLeft: 91,
        BackSlash: 92,
        BracketRight: 93,
        Quote: 39,
        DoubleQuote: 34,
        Digit0: 48,
        Digit1: 49,
        Digit2: 50,
        Digit3: 51,
        Digit4: 52,
        Digit5: 53,
        Digit6: 54,
        Digit7: 55,
        Digit8: 56,
        Digit9: 57,
        KeyA: 65,
        KeyB: 66,
        KeyC: 67,
        KeyD: 68,
        KeyE: 69,
        KeyF: 70,
        KeyG: 71,
        KeyH: 72,
        KeyI: 73,
        KeyJ: 74,
        KeyK: 75,
        KeyL: 76,
        KeyM: 77,
        KeyN: 78,
        KeyO: 79,
        KeyP: 80,
        KeyQ: 81,
        KeyR: 82,
        KeyS: 83,
        KeyT: 84,
        KeyU: 85,
        KeyV: 86,
        KeyW: 87,
        KeyX: 88,
        KeyY: 89,
        KeyZ: 90,
        Keya: 97,
        Keyb: 98,
        Keyc: 99,
        Keyd: 100,
        Keye: 101,
        Keyf: 102,
        Keyg: 103,
        Keyh: 104,
        Keyi: 105,
        Keyj: 106,
        Keyk: 107,
        Keyl: 108,
        Keym: 109,
        Keyn: 110,
        Keyo: 111,
        Keyp: 112,
        Keyq: 113,
        Keyr: 114,
        Keys: 115,
        Keyt: 116,
        Keyu: 117,
        Keyv: 118,
        Keyw: 119,
        Keyx: 120,
        Keyy: 121,
        Keyz: 122,
        " ": 32,
        "!": 33,
        "\"": 34,
        "#": 35,
        "$": 36,
        "%": 37,
        "&": 38,
        "'": 39,
        "(": 40,
        ")": 41,
        "*": 42,
        "+": 43,
        ",": 44,
        "-": 45,
        ".": 46,
        "/": 47,
        "0": 48,
        "1": 49,
        "2": 50,
        "3": 51,
        "4": 52,
        "5": 53,
        "6": 54,
        "7": 55,
        "8": 56,
        "9": 57,
        ":": 58,
        ";": 59,
        "<": 60,
        "=": 61,
        ">": 62,
        "?": 63,
        "@": 64,
        "A": 65,
        "B": 66,
        "C": 67,
        "D": 68,
        "E": 69,
        "F": 70,
        "G": 71,
        "H": 72,
        "I": 73,
        "J": 74,
        "K": 75,
        "L": 76,
        "M": 77,
        "N": 78,
        "O": 79,
        "P": 80,
        "Q": 81,
        "R": 82,
        "S": 83,
        "T": 84,
        "U": 85,
        "V": 86,
        "W": 87,
        "X": 88,
        "Y": 89,
        "Z": 90,
        "[": 91,
        "\\": 92,
        "]": 93,
        "^": 94,
        "_": 95,
        "`": 96,
        "a": 97,
        "b": 98,
        "c": 99,
        "d": 100,
        "e": 101,
        "f": 102,
        "g": 103,
        "h": 104,
        "i": 105,
        "j": 106,
        "k": 107,
        "l": 108,
        "m": 109,
        "n": 110,
        "o": 111,
        "p": 112,
        "q": 113,
        "r": 114,
        "s": 115,
        "t": 116,
        "u": 117,
        "v": 118,
        "w": 119,
        "x": 120,
        "y": 121,
        "z": 122,
        "{": 123,
        "|": 124,
        "}": 125,
        "~": 126
    }

    for (const key in local_keylayout) {
        keylayout[key] = local_keylayout[key]
    }

    return keylayout
}