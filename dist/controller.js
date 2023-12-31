// Note since this is wasm
// I now need to work with pointers
// variables which are pointers to be somehow denoted
// my method of doing this is to prefix them with a _

/**
 * Stores that load status of the window and the module
 */
const loaded = {
  window: false,
  Module: false,
};

/**
 * Event definitions
 */
const Event = {
  NONE: 0,
  PASS: 1,
  ILLEGAL_MOVE: 2,
  END: 3,
};

/**
 * Color definitions
 */
const Color = {
  WHITE: 0,
  BLACK: 1,
  UNDEFINED: 2,
};

/**
 * The main game function
 */
const main = () => {
  // Do not start until both the window and the module are loaded
  if (!(loaded.window && loaded.Module)) {
    return;
  }
  class Game {
    constructor() {
      // All of the function written in C++ wrapped in JS
      this.$reversi = Module.cwrap("createReversi", "number", []);
      this.$destroyReversi = Module.cwrap("destroyReversi", null, ["number"]);
      this.$getEvent = Module.cwrap("getEvent", "number", ["number"]);
      this.$getBoard = Module.cwrap("getBoard", "number", ["number"]);
      this.$getTurn = Module.cwrap("getTurn", "number", ["number"]);
      // Reference the the reversi object
      this._reversi = this.$reversi();
      this.$playerMove = Module.cwrap("playerMove", null, [
        "number",
        "number",
        "number",
      ]);
      this.$passCheck = Module.cwrap("passCheck", "number", ["number"]);
      this.$computerMove = Module.cwrap("computerMove", null, [
        "number",
        "number",
      ]);
      this.$countWhitePoints = Module.cwrap("countWhitePoints", "number", [
        "number",
      ]);
      this.$countBlackPoints = Module.cwrap("countBlackPoints", "number", [
        "number",
      ]);
    }
    get event() {
      // The the current event
      return this.$getEvent(this._reversi);
    }
    get board() {
      const startIndex = this.$getBoard(this._reversi);
      const endIndex = startIndex + 64; // 8x8 board
      const board = Module.HEAPU8.subarray(startIndex, endIndex);
      // Return the Uint8Array, which represents the board
      return board;
    }
    // Make a player move
    playerMove(x, y) {
      this.$playerMove(this._reversi, x, y);
    }
    /**
     * Check if the current player has to pass
     * @returns {boolean} if the current player has to pass
     */
    passCheck() {
      return Boolean(this.$passCheck(this._reversi));
    }
    /**
     * Memory cleanup
     * Call this when the game is over
     * or when the game is reset
     */
    destroy() {
      if (this._reversi !== null) {
        this.$destroyReversi(this._reversi);
        this._reversi = null;
      }
    }
    /**
     * Make the computer move
     */
    computerMove(depth) {
      this.$computerMove(this._reversi, depth);
    }
    /**
     * Get the current turn
     */
    get turn() {
      if (this.$getTurn(this._reversi) === Color.WHITE) {
        return "white";
      }
      return "black";
    }
    get whitePoints() {
      return this.$countWhitePoints(this._reversi);
    }
    get blackPoints() {
      return this.$countBlackPoints(this._reversi);
    }
  }
  /**
   * The renderer class used for rendering the game
   */
  class Renderer {
    newGame(start, clear) {
      // Remove all old items
      this.oldBoard = new Uint8Array(64).fill(Color.UNDEFINED);
      // Delete all children
      while (this.gameHeader.firstChild) {
        this.gameHeader.removeChild(this.gameHeader.firstChild);
      }
      while (this.gameFooter.firstChild) {
        this.gameFooter.removeChild(this.gameFooter.firstChild);
      }
      while (this.gameBody.firstChild) {
        this.gameBody.removeChild(this.gameBody.firstChild);
      }

      // Create options inside of game headers
      const options = document.createElement("div");
      options.id = "options";
      this.gameHeader.appendChild(options);
      // Create two divs inside of options
      // One for player v player
      // One for player v computer
      const playerVPlayer = document.createElement("div");
      playerVPlayer.id = "player-v-player";
      const playButton = document.createElement("button");

      playButton.innerHTML = `<i class="fa fa-user"></i> vs <i class="fa fa-user"></i>`
      playButton.addEventListener("click", () => {
        this.options.ai = false;
        start();
      });
      playerVPlayer.appendChild(playButton);
      options.appendChild(playerVPlayer);
      // Create a div for player v computer
      const playerVComputer = document.createElement("div");
      playerVComputer.id = "player-v-computer";
      const computerButton = document.createElement("button");
      computerButton.innerHTML = `<i class="fa fa-user"></i> vs <i class="fa fa-robot"></i>`
      computerButton.addEventListener("click", () => {
        this.options.ai = true
        start();
      });
      // Add input for depth allow 1 to 12
      const depthInput = document.createElement("input");
      depthInput.type = "number";
      depthInput.min = 1;
      depthInput.max = 12;
      depthInput.value = this.options.depth;
      depthInput.addEventListener("change", () => {
        this.options.depth = depthInput.value;
      });
      const depthLabel = document.createElement("label");
      depthLabel.innerText = "Depth";
      // Create a label for depth
      options.appendChild(playerVComputer);
      // Create a label and true false for will the player go first
      const first = document.createElement("div");
      first.id = "first";
      const firstLabel = document.createElement("label");
      firstLabel.innerText = "First";
      const firstInput = document.createElement("input");
      firstInput.type = "checkbox";
      firstInput.checked = this.options.first;
      firstInput.addEventListener("change", () => {
        this.options.first = !this.options.first;
      });
      first.appendChild(firstLabel);
      first.appendChild(firstInput);
      const aiOptions = document.createElement("div");
      aiOptions.id = "ai-options";
      playerVComputer.appendChild(computerButton);
      playerVComputer.appendChild(aiOptions);
      aiOptions.appendChild(depthLabel);
      aiOptions.appendChild(depthInput);
      aiOptions.appendChild(first);
      // Create a div that is the game footer

      // Create items for footer
      // p tag for information about the game
      // and a button to start a new game
      const footer = document.createElement("div");
      footer.id = "footer";
      const info = document.createElement("p");
      info.id = "info";
      this.info = info;
      const newGameButton = document.createElement("button");
      newGameButton.innerText = "New Game";
      newGameButton.addEventListener("click", () => {
        // Just simply refresh the page
        clear()
        this.newGame(start, clear);
      });
      footer.appendChild(info);
      footer.appendChild(newGameButton);
      this.gameFooter.appendChild(footer);
    }

    constructor() {
      this.onClick = () => {};
      this.gameContainer = document.getElementById("game-container");
      this.options = {
        ai: true,
        depth: 4,
        first: true,
      };
      // Delete all children
      while (this.gameContainer.firstChild) {
        this.gameContainer.removeChild(this.gameContainer.firstChild);
      }
      // Create a div that is the game header
      this.gameHeader = document.createElement("div");

      this.gameHeader.id = "game-header";
      this.gameHeader.innerText = "";
      this.gameContainer.appendChild(this.gameHeader);
      // Create a div that is the game body or board
      this.gameBody = document.createElement("div");
      this.gameBody.id = "game-body";
      this.gameContainer.appendChild(this.gameBody);

      // Create a div that is the game footer
      this.gameFooter = document.createElement("div");
      this.gameFooter.id = "game-footer";
      this.gameFooter.innerText = "";
      this.gameContainer.appendChild(this.gameFooter);

      this.oldBoard = new Uint8Array(64).fill(Color.UNDEFINED);

      const getGridSize = () => {
        // get the height of the game container
        const height = this.gameContainer.clientHeight;
        // get the width of the game container
        const width = this.gameContainer.clientWidth;
        // get the height of the footer
        const footerHeight = this.gameFooter.clientHeight;
        // get the height of the header
        const headerHeight = this.gameHeader.clientHeight;
        const actualHeight = height - footerHeight - headerHeight;
        const size = Math.min(actualHeight, width);
        return Math.floor(size / 8);
      };
      /**
       * Allow for the game board to be responsive
       */
      const resize = () => {
        // Set game-body grid template columns and rows
        const size = getGridSize();
        this.gameBody.style.gridTemplateColumns = `repeat(8, ${size}px)`;
        this.gameBody.style.gridTemplateRows = `repeat(8, ${size}px)`;
      };
      resize();
      window.addEventListener("resize", resize);
    }

    // The board accepts Uint8Array of size 64
    // it is a 8x8 board
    // 2 is empty
    // white is 0
    // black is 1
    setHeader(text) {
      this.gameHeader.innerText = text;
    }

    setFooter(text) {
      this.info.innerText = text;
    }

    setOld(board) {
      this.oldBoard = board;
    }

    renderBoard(board) {
      // Delete all children of game body
      const onClick = this.onClick;
      while (this.gameBody.firstChild) {
        this.gameBody.removeChild(this.gameBody.firstChild);
      }
      for (let i = 0; i < board.length; i++) {
        const tile = document.createElement("span");
        tile.classList.add("tile");
        if (this.oldBoard[i] !== board[i]) {
          tile.classList.add("new");
        }
        tile.id = `tile-${i}`;
        // add color attribute
        tile.setAttribute(
          "color",
          {
            [Color.WHITE]: "white",
            [Color.BLACK]: "black",
            [Color.UNDEFINED]: "empty",
          }[board[i]]
        );
        tile.addEventListener("click", () => {
          onClick(i);
        });
        this.gameBody.appendChild(tile);
      }
    }
  }
  let game = new Game();
  const rendered = new Renderer();
  // The game loop
  function loop() {
    const turn = game.turn;
    const passCheck = game.passCheck();
    const gameEnded = game.event === Event.END;
    // Simply pass if the player has to pass
    if (passCheck && !gameEnded) {
      rendered.setHeader(`${turn} has to pass`);
      setTimeout(() => loop(), 1000);
      return;
    }
    // Build the different messages for the header
    const buildMessage = () => {
      let message = "";
      switch (game.event) {
        case Event.PASS:
          message += "Player passed";
          break;
        case Event.ILLEGAL_MOVE:
          message += "Player illegal move";
          break;
        case Event.END:
          const whitePoints = game.whitePoints;
          const blackPoints = game.blackPoints;
          let winner = "";
          if(whitePoints > blackPoints) {
            winner = "White won";
          }else if(blackPoints > whitePoints) {
            winner = "Black won";
          }else {
            winner = "Tie";
          }
          message += "Game over.\n" + winner;
          break;
      }
      if (game.event !== Event.END) {
        message += `\nTurn: ${game.turn}`;
      }
      return message;
    };
    // Extract options
    const playAgainstAI = rendered.options.ai;
    const aiPlace = rendered.options.first ? "white" : "black";
    const aiTurn = game.turn === aiPlace && playAgainstAI;
    // If the aii is playing
    if (aiTurn) {
      // Then remove the ability to click for the human
      rendered.onClick = () => {};
      // Notify the user that the computer is thinking
      rendered.setHeader(
        buildMessage(game.event) + "\nComputer is thinking..."
      );
      // Render the board before the computer move
      rendered.renderBoard(game.board);
      // Make the computer move after 500ms
      setTimeout(() => {
        const oldBoard = new Uint8Array(game.board);
        rendered.setOld(oldBoard);
        game.computerMove(rendered.options.depth);
        loop();
      }, 500);
      return;
    } else {
      if(!gameEnded) {
        // Allow the user to click if the game has not ended
        // Instantly render the new board if the user clicks
        rendered.onClick = (i) => {
          const x = i % 8;
          const y = Math.floor(i / 8);
          // Create board deep copy
          const oldBoard = new Uint8Array(game.board);
          rendered.setOld(oldBoard);
          game.playerMove(x, y);
          setTimeout(() => loop(), 0);
        };
      } else {
        // Forbid the user from clicking if the game has ended
        rendered.onClick = () => {};
      }
    }
    // Set the information about the game
    rendered.setHeader(buildMessage(game.event));
    rendered.renderBoard(game.board);
    rendered.setFooter(
        `White: ${game.whitePoints} Black: ${game.blackPoints}`
    );
  }
  rendered.newGame(() => {
    // Clean up the game before a new one
    game.destroy();
    // Renderer clear header
    while (rendered.gameHeader.firstChild) {
      rendered.gameHeader.removeChild(rendered.gameHeader.firstChild);
    }
    game = new Game();
    loop();
  }, ()=> {
    game.destroy();
  });
};

window.onload = () => {
  // Either the window is loaded or the module is loaded
  // Call main just in case both are loaded
  loaded.window = true;
  main();
};

Module.onRuntimeInitialized = () => {
  // Either the window is loaded or the module is loaded
  // Call main just in case both are loaded
  loaded.Module = true;
  main();
};
