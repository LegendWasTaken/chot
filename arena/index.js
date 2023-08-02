const fs = require('fs');
const {Chess} = require("chess.js");
const {Engine} = require('node-uci');

const initializeEngine = (path) => {
    return new Promise(async (resolve, reject) => {
        let engine = new Engine(path);
        await engine.init();
        await engine.isready()
        resolve(engine);
    });
}

let openings = fs.readFileSync(`openings/8moves_v3.txt`, 'utf-8').split('\n').map(fen => new Chess(fen));
const chooseRandomOpening = () => {
    return openings[Math.floor(Math.random()*openings.length)];
}


let engineOne = initializeEngine("engines/chot-uci");
let engineTwo = initializeEngine("engines/chot-uci");
let engines = Promise.all([engineOne, engineTwo]);
engines.then(async ([one, two]) => {
    let getWinner = (game) => {
        if (game.isDraw()) {
            return "draw";
        } else {
            if (game.turn() === "w") {
                return "b";
            } else {
                return "w";
            }
        }
    }

    try {
        for (let i = 0; i < 10; i++) {
            const game = chooseRandomOpening();
            console.log(`Engines playing - ${game.fen()}`);

            while (!game.isGameOver()) {
                await one.position(game.fen());
                let white = await one.go({nodes: 300}); // nodes is currently unused by engine
                game.move(white.bestmove);

                await two.position(game.fen());
                let black = await two.go({nodes: 300}); // unused - same as above
                game.move(black.bestmove);
            }

            const winner = getWinner(game);
            console.log(`Engines finished - Winner: ${winner}`);
        }
    } catch (e) {
        console.log("uh oh");
        console.error(e);
    }
});
