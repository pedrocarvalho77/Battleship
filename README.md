# Battleship

Two-player Battleship game for the terminal, written in C (C99). Built as a university course project.

## Build and play

Requires `gcc` and `make`.

```bash
make        # builds build/libgame.a and build/engine
./build/engine    # or: make run
make clean
```

## Rules

- Random setup (`R`) or manual setup (`M`). Random setup picks a random map size; manual setup lets you choose between 20 and 40.
- Both players agree on how many boats of each of the 5 types to use: `T`, `U`, `||`, `|` and `L`. In manual setup the boats are placed one by one.
- Hot-seat game: both players share the keyboard. During manual setup the screen is cleared before and after each player places their boats, and between turns the game waits for Enter and clears the screen.
- Players 1 and 2 take turns shooting at (x, y) coordinates. Each turn shows what you know of the opponent's waters (`X` hit, `o` miss, `.` unknown); the opponent's boats are never shown.
- Each boat piece hit removes 1 hp from the opponent, and the game tells you when a boat is sunk. Starting hp is the total number of pieces of that player's boats (for example, a `|` boat has 5 pieces).
- The first player to bring the opponent to 0 hp wins, and the game offers a rematch.
- A boat is placed by the top-left corner of its shape; only the boat's pieces need to fit inside the map and not overlap other boats.

## Map storage

At startup the game asks whether to store the map in arrays (`A`) or quadtrees (`Q`).

Both modes play a full game. In quadtree mode the tree decides hits and misses; the array map is only used to draw the boats during setup.

## Layout

- `src/`: `engine.c` (main), `setup.c` (game setup and prompts), `placement.c` (placing boats, manual and random), `game.c` (game loop), `player.c`, `boat.c`, `cell.c`, `point.c`, `quadtree.c`, `logo.c`
- `include/`: the matching headers
- `build/`: compiled output (git-ignored)
