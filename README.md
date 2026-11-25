# Battleship

Two-player Battleship game for the terminal, written in C (C99). Built as a university course project.

## Build and play

Requires `gcc` and `make`.

```bash
make        # builds libgame.a and the engine executable
./engine    # or: make run
make clean
```

## Rules

- Random setup (`R`) or manual setup (`M`). Random setup picks a random map size; manual setup lets you choose between 20 and 40.
- Both players agree on how many boats of each of the 5 types to use: `T`, `U`, `||`, `|` and `L`. In manual setup the boats are placed one by one.
- Players 1 and 2 take turns shooting at (x, y) coordinates.
- Each boat piece hit removes 1 hp from the opponent. Starting hp is the total number of pieces of that player's boats (for example, a `|` boat has 5 pieces).
- The first player to bring the opponent to 0 hp wins.

## Map storage

At startup the game asks whether to store the map in arrays (`A`) or quadtrees (`Q`).

**Known state:** array mode (`A`) works end to end. Quadtree mode (`Q`) is unfinished: each player's tree is never initialised, so no boats are recorded and no shot can hit. Use `A`.

## Layout

`engine.c` (main), `setup.c` (game setup), `game.c` (game loop), `player.c`, `boat.c`, `cell.c`, `point.c`, `quadtree.c`, `logo.c`.
