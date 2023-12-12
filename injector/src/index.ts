import memoryjs from 'memoryjs';
import path from 'node:path';
import { PROCESS_NAME } from './constants/game';
import { getValueWithOffsets } from './utils/memory';

const __dirname = path.resolve(path.dirname('')); 

// const MOUSE_X_OFFSETS = [modBaseAddr + 0x1B695A0, 0x10, 0x38, 0x170, 0x48, 0x30, 0x350] as const
// const MOUSE_Y_OFFSETS = [modBaseAddr + 0x01CDEB50, 0x38, 0x198, 0x0, 0x50, 0x8, 0x30, 0x354] as const

// while (true) {
//     const mouseX = getValueWithOffsets(handle, MOUSE_X_OFFSETS, memoryjs.UINT16)
//     const mouseY = getValueWithOffsets(handle, MOUSE_Y_OFFSETS, memoryjs.UINT16)
//     const cursorVisible = getValueWithOffsets(handle, CURSOR_VISIBLE_OFFSETS, memoryjs.UINT16)

//     console.log({ mouseX, mouseY, cursorVisible })
// }

// setValueWithOffsets(handle, CURSOR_VISIBLE_OFFSETS, 0x00000002, memoryjs.UINT16)

let isRunning = false

console.log('Waiting for game to start...');

while (!isRunning) {
    let process: memoryjs.Process | undefined;

    try {
        process = memoryjs.openProcess(PROCESS_NAME);
    } catch (error) {
        // Do nothing
    }

    if (!process) {
        continue;
    }

    const { handle, modBaseAddr } = process;

    const CURSOR_VISIBLE_OFFSETS = [modBaseAddr + 0x1D059E8, 0x360] as const

    const cursorVisible = getValueWithOffsets(handle, CURSOR_VISIBLE_OFFSETS, memoryjs.UINT16)

    // Wait for the cursor to be visible
    if (cursorVisible !== 0x2) {
        continue;
    }

    const success = memoryjs.injectDll(handle, path.join(__dirname, '..', 'core', 'out', 'build', 'x64-debug', 'core.dll'));

    isRunning = success;

    if (success) {
        console.log('Injected successfully!');
    }
}