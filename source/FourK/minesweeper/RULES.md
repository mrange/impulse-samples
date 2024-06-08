# Minesweeper 2024

1. **Modified Minesweeper Rules**
2. Each square displays a number indicating the count of adjacent bombs.
3. Revealing a square is done by clicking the left mouse button.
4. Revealing a square containing a bomb ends the game.
5. If a revealed square has no adjacent bombs, adjacent squares are automatically revealed.
6. Marking a square as a possible bomb is done by clicking the right mouse button. This action has no impact on gameplay.
7. The game board is considered complete when all non-bomb squares are revealed.
8. The "Left" indicator at the top displays the number of unrevealed squares remaining.
9. Scoring begins at 1000 points and decreases over a period of 60 seconds.
10. Successfully clearing a board awards a new score equal to the current score plus 1000 times the number of boards cleared.
11. Upon clearing a board, a "locked score" is calculated by halving the new score. If the resulting score is lower than the previous locked score, the previous locked score is retained. The score then decreases from the new score to the locked score over 60 seconds.
12. On game over, your final score is displayed.
13. Pressing the "R" key restarts the game, allowing you to start fresh with a new board.
14. Pressing the "Escape" key quits the game.
