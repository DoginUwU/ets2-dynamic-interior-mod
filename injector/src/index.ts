import memoryjs from 'memoryjs';
import { PROCESS_NAME } from './constants/game';
import { getValueWithOffsets, setValueWithOffsets } from './utils/memory';
import path from 'node:path';
const __dirname = path.resolve(path.dirname('')); 

const { handle, modBaseAddr } = memoryjs.openProcess(PROCESS_NAME);

const MOUSE_X_OFFSETS = [modBaseAddr + 0x1B695A0, 0x10, 0x38, 0x170, 0x48, 0x30, 0x350] as const
const MOUSE_Y_OFFSETS = [modBaseAddr + 0x01CDEB50, 0x38, 0x198, 0x0, 0x50, 0x8, 0x30, 0x354] as const
// const CURSOR_VISIBLE_OFFSETS = [modBaseAddr + 0x01D059E8, 0x360] as const

// while (true) {
//     const mouseX = getValueWithOffsets(handle, MOUSE_X_OFFSETS, memoryjs.UINT16)
//     const mouseY = getValueWithOffsets(handle, MOUSE_Y_OFFSETS, memoryjs.UINT16)
//     const cursorVisible = getValueWithOffsets(handle, CURSOR_VISIBLE_OFFSETS, memoryjs.UINT16)

//     console.log({ mouseX, mouseY, cursorVisible })
// }

// setValueWithOffsets(handle, CURSOR_VISIBLE_OFFSETS, 0x00000002, memoryjs.UINT16)
const success = memoryjs.injectDll(handle, path.join(__dirname, '..', 'core', 'out', 'build', 'x64-debug', 'core.dll'));

console.log({ success })