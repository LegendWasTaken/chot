const fs = require('fs');
const chess = require('chess.js');
const {Chess} = require("chess.js");
fs.readdirSync('openings/raw').forEach(file => {
    console.log(`Processing opening file '${file}'`);
    const content = fs.readFileSync(`openings/raw/${file}`, 'utf-8');
    const matches = content.matchAll('(1\\. [a-zA-Z0-9.+\\s-]* 1\\/2-1\\/2)');
    let fens = [];
    for (const match of matches) {
        const pgn = match[0];
        let pos = new Chess();
        pos.loadPgn(pgn);
        fens.push(pos.fen());
    }
    fs.writeFileSync(`openings/${file.replaceAll("pgn", "txt")}`, fens.join('\n'));
});
