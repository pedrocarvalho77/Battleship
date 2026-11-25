# Battleship

Jogo de batalha naval para dois jogadores, em C (C99), no terminal. Trabalho de uma unidade curricular.

## Compilar e jogar

Precisa de `gcc` e `make`.

```bash
make        # compila libgame.a e o executável engine
./engine    # ou: make run
make clean
```

## Regras

- Setup aleatório (`R`) ou manual (`M`). No aleatório o mapa tem tamanho aleatório; no manual escolhe-se entre 20 e 40.
- Escolhe-se, de comum acordo, o número de barcos de cada um dos 5 tipos: `T`, `U`, `||`, `|` e `L`. No setup manual os barcos são colocados um a um.
- Os jogadores 1 e 2 jogam à vez, indicando as coordenadas (x, y) do tiro.
- Cada peça acertada retira 1 de hp ao adversário. O hp inicial é o total de peças dos seus barcos (por exemplo, um barco `|` tem 5 peças).
- Ganha quem deixar o adversário com hp 0.

## Armazenamento do mapa

No arranque pergunta-se se o mapa é guardado em arrays (`A`) ou quadtrees (`Q`).

**Estado conhecido:** o modo `A` (arrays) funciona de ponta a ponta. O modo `Q` (quadtrees) está incompleto: a árvore de cada jogador nunca é inicializada, por isso não regista barcos e não há acertos. Use `A`.

## Estrutura

`engine.c` (main), `setup.c` (configuração do jogo), `game.c` (ciclo de jogo), `player.c`, `boat.c`, `cell.c`, `point.c`, `quadtree.c`, `logo.c`.
