// Note since this is wasm 
// I now need to work with pointers
// variables which are pointers to be somehow denoted
// my method of doing this is to prefix them with a _


const loaded = {
    window: false,
    Module: false
}

const Event = {
    NONE: 0,
    PASS: 1,
    ILLEGAL_MOVE: 2,
    END: 3
}

const Color = {
    WHITE: 0,
    BLACK: 1,
    UNDEFINED: 2
}

const main = () => {
    if (!(loaded.window && loaded.Module)) {
        return
    }
    class Game {
        constructor() {
            this._reversi = Module.cwrap('createReversi', 'number', [])
            this.$destroyReversi = Module.cwrap('destroyReversi', null, ['number'])
            this.$getEvent = Module.cwrap('getEvent', 'number', ['number'])
            this.$getBoard = Module.cwrap('getBoard', 'number', ['number'])
            this.$getTurn = Module.cwrap('getTurn', 'number', ['number'])
            this._reversi = this._reversi()
            this.$playerMove = Module.cwrap('playerMove', null, ['number', 'number', 'number'])
            this.$passCheck = Module.cwrap('passCheck', 'number', ['number'])
            this.$computerMove = Module.cwrap('computerMove', null, ['number', 'number'])
        }
        get event() {
            return this.$getEvent(this._reversi)
        }
        get board() {
            const startIndex = this.$getBoard(this._reversi)
            const endIndex = startIndex + 64 // 8x8 board
            const board = Module.HEAPU8.subarray(startIndex, endIndex);
            return board
        }
        playerMove(x, y) {
            this.$playerMove(this._reversi, x, y)
        }
        passCheck() {
            return Boolean(this.$passCheck(this._reversi))
        }
        destroy() {
            if(this._reversi !== null) {
                this.$destroyReversi(this._reversi)
                this._reversi = null
            }
        }
        computerMove(depth){
            this.$computerMove(this._reversi, depth)
        }
        get turn() {
            if (this.$getTurn(this._reversi) === Color.WHITE) {
                return 'white'
            }
            return 'black'
        }
    }
    class Renderer {
        
        constructor() {

            this.gameContainer = document.getElementById('game-container')
            // Delete all children
            while (this.gameContainer.firstChild) {
                this.gameContainer.removeChild(this.gameContainer.firstChild);
            }
            // Create a div that is the game header
            this.gameHeader = document.createElement('div')

            this.gameHeader.id = 'game-header'
            this.gameHeader.innerText = ''
            this.gameContainer.appendChild(this.gameHeader)
            // Create a div that is the game body or board
            this.gameBody = document.createElement('div')
            this.gameBody.id = 'game-body'
            this.gameBody.innerText = 'Body'
            this.gameContainer.appendChild(this.gameBody)

            // Create a div that is the game footer
            this.gameFooter = document.createElement('div')
            this.gameFooter.id = 'game-footer'
            this.gameFooter.innerText = ''
            this.gameContainer.appendChild(this.gameFooter)

            this.oldBoard = new Uint8Array(64).fill(Color.UNDEFINED)
        }

        // The board accepts Uint8Array of size 64
        // it is a 8x8 board
        // 2 is empty
        // white is 0
        // black is 1
        setHeader(text) {
            this.gameHeader.innerText = text
        }

        setOld(board){
            this.oldBoard = board
        }

        renderBoard(board, onClick) {
            // Delete all children of game body
            while (this.gameBody.firstChild) {
                this.gameBody.removeChild(this.gameBody.firstChild);
            }
            for(let i = 0; i < board.length; i++) {
                const tile = document.createElement('span')
                tile.classList.add('tile')
                if(this.oldBoard[i] !== board[i]) {
                    tile.classList.add('new')
                }
                tile.id = `tile-${i}`
                // add color attribute
                tile.setAttribute('color', {
                    [Color.WHITE]: 'white',
                    [Color.BLACK]: 'black',
                    [Color.UNDEFINED]: 'empty'
                }[board[i]])
                tile.addEventListener('click', () => {
                    onClick(i)
                })
                this.gameBody.appendChild(tile)
            }
        }
    }
    const game = new Game()
    const rendered = new Renderer()
    function loop() {
        const passCheck = game.passCheck()
        const gameEnded = game.event === Event.END
        if(passCheck && !gameEnded) {
            setTimeout(() => loop(), 0)
            return
        }
        const buildMessage = () => {
            let message = ''
            switch(game.event) {
                case Event.PASS:
                    message += 'Player passed'
                    break
                case Event.ILLEGAL_MOVE:
                    message += 'Player illegal move'
                    break
                case Event.END:
                    message += 'Game end'
                    break
            }
            if(game.event !== Event.END) {
                message += `\nTurn: ${game.turn}`
            }
            return message
        }
        if(game.turn === 'black') {
            rendered.setHeader(buildMessage(game.event) + '\nComputer is thinking...')
            rendered.renderBoard(game.board, () => {})

            setTimeout(() => {
                const oldBoard = new Uint8Array(game.board);
                rendered.setOld(oldBoard)
                game.computerMove(7)
                loop()
            }, 500)
            return
        }
        rendered.setHeader(buildMessage(game.event))
        rendered.renderBoard(game.board, (i) => {
            if(!gameEnded) {
                const x = i % 8
                const y = Math.floor(i / 8)
                // Create board deep copy
                const oldBoard = new Uint8Array(game.board);
                rendered.setOld(oldBoard)
                game.playerMove(x, y)
                setTimeout(() => loop(), 0)
            } else {
                game.destroy()
            }
        })
    }
    loop()
}

window.onload = () => {
    loaded.window = true;
    main()
}

Module.onRuntimeInitialized = () => {
    loaded.Module = true;
    main()
}
