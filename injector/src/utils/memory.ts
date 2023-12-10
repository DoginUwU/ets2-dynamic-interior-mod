import memoryjs from "memoryjs";

export function getValueWithOffsets (handle: number, offsets: readonly number[], resultType: number) {
    let value: unknown = null;

    joinOffsets(handle, offsets, pointer => {
        value = memoryjs.readMemory(handle, pointer, resultType);
    });

    return value;
}

export function setValueWithOffsets(handle: number, offsets: readonly number[], value: any, dataType: number) {
    joinOffsets(handle, offsets, pointer => {
        memoryjs.writeMemory(handle, pointer, value, dataType);
    });
}

function joinOffsets(handle: number, offsets: readonly number[], callback: (pointer: number) => void) {
    let pointer = null;

    for (let i = 0; i < offsets.length; i++) {
        const offset = offsets[i];
        const isLast = i === offsets.length - 1;

        if (isLast) {
            callback(Number(pointer) + offset);
        } else {
            if (pointer === null) {
                pointer = memoryjs.readMemory(handle, offset, memoryjs.PTR);
            } else {
                pointer = memoryjs.readMemory(handle, Number(pointer) + offset, memoryjs.PTR);
            }
        }
    }
}