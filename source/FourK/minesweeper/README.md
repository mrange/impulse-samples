# Minesweeper

## Commands

### Generate SPIR-V human form

Requires `layout(location=0)` on uniforms.

```bash
glslangValidator.exe shader.frag -G -H
```


## Ideas

### Scoring

1. Score is constantly ticking down
2. Score down tick speed is depending on how many revealed you have
3. Perhaps score can't tick below some limit? Like last board score?
3. Clearing a board awards points
4. You need to clear boards in a certain time
5. Triggering in a bomb don't end the game but hurts score
6. When you are happy with your score you end the game to collect the score
