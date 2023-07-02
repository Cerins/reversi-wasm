// Note since this is wasm 
// I now need to work with pointers
// variables which are pointers to be somehow denoted
// my method of doing this is to prefix them with a _

Module.onRuntimeInitialized = async _ => {
    console.log('Hello from controller.js')

    const createReversi = Module.cwrap('createReversi', 'number', [])
    const destroyReversi = Module.cwrap('destroyReversi', null, ['number'])
    const getEvent = Module.cwrap('getEvent', 'number', ['number'])
    const getBoard = Module.cwrap('getBoard', 'number', ['number'])
    const main = Module.cwrap('main', null, ['number'])

    const _reversi = createReversi()
    console.log('reversi', _reversi)
    const event = getEvent(_reversi)
    const _board = getBoard(_reversi)
    const startIndex = _board / 4
    const endIndex = startIndex + 64 // 8x8 board
    // Yea the board use 32 bit integers, absolutely not needed, but i was lazy
    // TODO: change to 8 bit integers someday
    const board = Module.HEAP32.subarray(startIndex, endIndex);
    console.log('board', board)
    main();
    destroyReversi(_reversi)
}
