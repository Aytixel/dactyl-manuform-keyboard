const MOD_ALT_GRAPH = MOD_RIGHT_ALT

const keylayout_fr_fr = {
    0x04: ["Keyq", "q"],                //
    // 0x05: ["Keyb", "b"],
    // 0x06: ["Keyc", "c"],
    // 0x07: ["Keyd", "d"],
    // 0x08: ["Keye", "e"],
    // 0x09: ["Keyf", "f"],
    // 0x0A: ["Keyg", "g"],
    // 0x0B: ["Keyh", "h"],
    // 0x0C: ["Keyi", "i"],
    // 0x0D: ["Keyj", "j"],
    // 0x0E: ["Keyk", "k"],
    // 0x0F: ["Keyl", "l"],
    0x10: ["Comma", ","],               //
    // 0x11: ["Keyn", "n"],
    // 0x12: ["Keyo", "o"],
    // 0x13: ["Keyp", "p"],
    0x14: ["Keya", "a"],                //
    // 0x15: ["Keyr", "r"],
    // 0x16: ["Keys", "s"],
    // 0x17: ["Keyt", "t"],
    // 0x18: ["Keyu", "u"],
    // 0x19: ["Keyv", "v"],
    0x1A: ["Keyz", "z"],                //
    // 0x1B: ["Keyx", "x"],
    // 0x1C: ["Keyy", "y"],
    0x1D: ["Keyw", "w"],                //
    0x1E: ["Ampersand", "&"],           //
    0x1F: ["EAcute", "é"],              //
    0x20: ["DoubleQuote", "\""],        //
    0x21: ["Quote", "'"],               //
    0x22: ["ParenthesisLeft", "("],     //
    0x23: ["Minus", "-"],               //
    0x24: ["EGrave", "è"],              //
    0x25: ["Underscore", "_"],          //
    0x26: ["CCedilla", "ç"],            //
    0x27: ["AGrave", "à"],              //
    // 0x28: ["Enter"],
    // 0x29: ["Escape"],
    // 0x2A: ["Backspace"],
    // 0x2B: ["Tab"],
    // 0x2C: ["Space", " "],
    0x2D: ["ParenthesisRight", ")"],    //
    // 0x2E: ["Equal", "="],
    0x2F: ["Caret", "^"],               //
    0x30: ["Dollar", "$"],              //
    0x31: ["AngleBracketLeft", "<"],    //
    // 0x32
    0x33: ["Keym", "m"],                //
    0x34: ["UGrave", "ù"],              //
    0x35: ["SuperScript_2", "²"],       //
    0x36: ["SemiColon", ";"],           //
    0x37: ["Colon", ":"],               //
    0x38: ["ExclamationMark", "!"],     //
    // 0x39: ["CapsLock"],
    // 0x3A: ["F1"],
    // 0x3B: ["F2"],
    // 0x3C: ["F3"],
    // 0x3D: ["F4"],
    // 0x3E: ["F5"],
    // 0x3F: ["F6"],
    // 0x40: ["F7"],
    // 0x41: ["F8"],
    // 0x42: ["F9"],
    // 0x43: ["F10"],
    // 0x44: ["F11"],
    // 0x45: ["F12"],
    // 0x46: ["PrintScreen"],
    // 0x47: ["ScrollLock"],
    // 0x48: ["Pause"],
    // 0x49: ["Insert"],
    // 0x4A: ["Home"],
    // 0x4B: ["PageUp"],
    // 0x4C: ["Delete"],
    // 0x4D: ["End"],
    // 0x4E: ["PageDown"],
    // 0x4F: ["ArrowRight"],
    // 0x50: ["ArrowLeft"],
    // 0x51: ["ArrowDown"],
    // 0x52: ["ArrowUp"],
    // 0x53: ["KeypadNumLock"],
    // 0x54: ["KeypadDivide"],
    // 0x55: ["KeypadMultiply"],
    // 0x56: ["KeypadSubtract"],
    // 0x57: ["KeypadAdd"],
    // 0x58: ["KeypadEnter"],
    // 0x59: ["Keypad1"],
    // 0x5A: ["Keypad2"],
    // 0x5B: ["Keypad3"],
    // 0x5C: ["Keypad4"],
    // 0x5D: ["Keypad5"],
    // 0x5E: ["Keypad6"],
    // 0x5F: ["Keypad7"],
    // 0x60: ["Keypad8"],
    // 0x61: ["Keypad9"],
    // 0x62: ["Keypad0"],
    // 0x63: ["KeypadPeriod"],

    [0x04 | MOD_LEFT_SHIFT]: ["KeyQ", "Q"],                 //
    // [0x05 | MOD_LEFT_SHIFT]: ["KeyB", "B"],
    // [0x06 | MOD_LEFT_SHIFT]: ["KeyC", "C"],
    // [0x07 | MOD_LEFT_SHIFT]: ["KeyD", "D"],
    // [0x08 | MOD_LEFT_SHIFT]: ["KeyE", "E"],
    // [0x09 | MOD_LEFT_SHIFT]: ["KeyF", "F"],
    // [0x0A | MOD_LEFT_SHIFT]: ["KeyG", "G"],
    // [0x0B | MOD_LEFT_SHIFT]: ["KeyH", "H"],
    // [0x0C | MOD_LEFT_SHIFT]: ["KeyI", "I"],
    // [0x0D | MOD_LEFT_SHIFT]: ["KeyJ", "J"],
    // [0x0E | MOD_LEFT_SHIFT]: ["KeyK", "K"],
    // [0x0F | MOD_LEFT_SHIFT]: ["KeyL", "L"],
    [0x10 | MOD_LEFT_SHIFT]: ["QuestionMark", "?"],         //
    // [0x11 | MOD_LEFT_SHIFT]: ["KeyN", "N"],
    // [0x12 | MOD_LEFT_SHIFT]: ["KeyO", "O"],
    // [0x13 | MOD_LEFT_SHIFT]: ["KeyP", "P"],
    [0x14 | MOD_LEFT_SHIFT]: ["KeyA", "A"],                 //
    // [0x15 | MOD_LEFT_SHIFT]: ["KeyR", "R"],
    // [0x16 | MOD_LEFT_SHIFT]: ["KeyS", "S"],
    // [0x17 | MOD_LEFT_SHIFT]: ["KeyT", "T"],
    // [0x18 | MOD_LEFT_SHIFT]: ["KeyU", "U"],
    // [0x19 | MOD_LEFT_SHIFT]: ["KeyV", "V"],
    [0x1A | MOD_LEFT_SHIFT]: ["KeyZ", "Z"],                 //
    // [0x1B | MOD_LEFT_SHIFT]: ["KeyX", "X"],
    // [0x1C | MOD_LEFT_SHIFT]: ["KeyY", "Y"],
    [0x1D | MOD_LEFT_SHIFT]: ["KeyW", "W"],                 //
    [0x1E | MOD_LEFT_SHIFT]: ["Digit1", "1"],               //
    [0x1F | MOD_LEFT_SHIFT]: ["Digit2", "2"],               //
    [0x20 | MOD_LEFT_SHIFT]: ["Digit3", "3"],               //
    [0x21 | MOD_LEFT_SHIFT]: ["Digit4", "4"],               //
    [0x22 | MOD_LEFT_SHIFT]: ["Digit5", "5"],               //
    [0x23 | MOD_LEFT_SHIFT]: ["Digit6", "6"],               //
    [0x24 | MOD_LEFT_SHIFT]: ["Digit7", "7"],               //
    [0x25 | MOD_LEFT_SHIFT]: ["Digit8", "8"],               //
    [0x26 | MOD_LEFT_SHIFT]: ["Digit9", "9"],               //
    [0x27 | MOD_LEFT_SHIFT]: ["Digit0", "0"],               //
    // 0x28 - 0x2C
    [0x2D | MOD_LEFT_SHIFT]: ["Degree", "°"],               //
    // [0x2E | MOD_LEFT_SHIFT]: ["Plus", "+"],
    [0x2F | MOD_LEFT_SHIFT]: ["Diaeresis", "¨"],            //
    [0x30 | MOD_LEFT_SHIFT]: ["Pound", "£"],                //
    [0x31 | MOD_LEFT_SHIFT]: ["AngleBracketRight", ">"],    //
    // 0x32
    [0x33 | MOD_LEFT_SHIFT]: ["KeyM", "M"],                 //
    [0x34 | MOD_LEFT_SHIFT]: ["Percent", "%"],              //
    [0x35 | MOD_LEFT_SHIFT]: [],                            //
    [0x36 | MOD_LEFT_SHIFT]: ["Period", "."],               //
    [0x37 | MOD_LEFT_SHIFT]: ["Slash", "/"],                //
    [0x38 | MOD_LEFT_SHIFT]: ["Section", "§"],              //
    // 0x39 - 0x52
    // [0x53 | MOD_LEFT_SHIFT]: ["KeypadClear"],
    // [0x59 | MOD_LEFT_SHIFT]: ["KeypadEnd"],
    // [0x5A | MOD_LEFT_SHIFT]: ["KeypadArrowDown"],
    // [0x5B | MOD_LEFT_SHIFT]: ["KeypadPageDown"],
    // [0x5C | MOD_LEFT_SHIFT]: ["KeypadArrowLeft"],
    // [0x5E | MOD_LEFT_SHIFT]: ["KeypadArrowRight"],
    // [0x5F | MOD_LEFT_SHIFT]: ["KeypadHome"],
    // [0x60 | MOD_LEFT_SHIFT]: ["KeypadArrowUp"],
    // [0x61 | MOD_LEFT_SHIFT]: ["KeypadPageUp"],
    // [0x62 | MOD_LEFT_SHIFT]: ["KeypadInsert"],
    // [0x63 | MOD_LEFT_SHIFT]: ["KeypadDelete"],

    // 0x04 - 0x07
    [0x08 | MOD_ALT_GRAPH]: ["Euro", "€"],                  //
    // 0x09 - 0x1E
    [0x1F | MOD_ALT_GRAPH]: ["Tilde", "~"],                 //
    [0x20 | MOD_ALT_GRAPH]: ["Hashtag", "#"],               //
    [0x21 | MOD_ALT_GRAPH]: ["CurlyBracketLeft", "{"],      //
    [0x22 | MOD_ALT_GRAPH]: ["BracketLeft", "["],           //
    [0x23 | MOD_ALT_GRAPH]: ["Pipe", "|"],                  //
    [0x24 | MOD_ALT_GRAPH]: ["BackQuote", "`"],             //
    [0x25 | MOD_ALT_GRAPH]: ["BackSlash", "\\"],            //
    [0x26 | MOD_ALT_GRAPH]: [],                             //
    [0x27 | MOD_ALT_GRAPH]: ["At", "@"],                    //
    // 0x28 - 0x2C
    [0x2D | MOD_ALT_GRAPH]: ["BracketRight", "]"],          //
    [0x2E | MOD_ALT_GRAPH]: ["CurlyBracketRight", "}"],     //
    // 0x2F
    [0x30 | MOD_ALT_GRAPH]: ["Currency", "¤"],              //
    // 0x31 - 0x63

    // [0xE0 | MOD_LEFT_CTRL]: ["ControlLeft"],
    // [0xE1 | MOD_LEFT_SHIFT]: ["ShiftLeft"],
    // [0xE2 | MOD_LEFT_ALT]: ["AltLeft"],
    // [0xE3 | MOD_LEFT_GUI]: ["MetaLeft"],
    // [0xE4 | MOD_RIGHT_CTRL]: ["ControlRight"],
    // [0xE5 | MOD_RIGHT_SHIFT]: ["ShiftRight"],
    [0xE6 | MOD_ALT_GRAPH]: ["AltGraph"],                   //
    // [0xE7 | MOD_RIGHT_GUI]: ["MetaRight"],
}