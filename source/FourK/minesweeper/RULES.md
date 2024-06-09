Here is the updated ruleset with the new flag functionality:

# Minesweeper 2024

1. **Modified Minesweeper Rules**
2. The board is 12x12 squares and 16 bombs are placed randomly on the board.
3. Each square displays a number indicating the count of adjacent bombs.
4. Revealing a square is done by clicking the left mouse button.
5. Revealing a square containing a bomb ends the game.
6. If a revealed square has no adjacent bombs, adjacent squares are automatically revealed.
7. Marking a square with a flag is done by clicking the right mouse button. The flag indicates that you believe the square contains a bomb.
8. If you click a revealed square with a number and the number matches the flags around it, all remaining squares around the number will be revealed. If any flags are misplaced, a bomb will be revealed and the game ends.
9. The game board is considered complete when all non-bomb squares are revealed.
10. The "Left" indicator at the top displays the number of unrevealed squares remaining.
11. Scoring begins at 1000 points and decreases over a period of 60 seconds.
12. Successfully clearing a board awards a new score equal to the current score plus 1000 times the number of boards cleared.
13. Upon clearing a board, a "locked score" is calculated by halving the new score. If the resulting score is lower than the previous locked score, the previous locked score is retained. The score then decreases from the new score to the locked score over 60 seconds.
14. On game over, your final score is displayed.
15. Pressing the "R" key restarts the game, allowing you to start fresh with a new board.
16. Pressing the "Escape" key quits the game.