Snake by Andrea Covre

<<< DESCRIPTION >>> ---------------------------------------------------------------------

The game is just a recreation of the popular game Snake


<<< CONTROLS >>> -------------------------------------------------------------------------

→: go right
←: go left
↑: go up
↓: go down

Start: start the game at the beginning and after losing
Select: Reset the game at any time to its start page


<<< Dynamics of the Game >>> --------------------------------------------------------------

1. The snake will increase in length every time a target gets eaten (object collision)
2. The snake cannot hit the walls around the perimeter of the screen (object collision)
3. There are 5 level of speed, the snake will get faster every time 5 targets get eaten
4. The snake cannot crossover/hit itself (object collision)


<<< IMAGES >>> -----------------------------------------------------------------------------

1. Start Scene (full screen)
2. Game Over Scene (full screen)
3. Target image (4x4) it resembles a bug


<<< TEXT >>> -------------------------------------------------------------------------------

After losing the game the score (based on how many targets have been eaten) will be 
shown in the Game Over scene


<<< IPLEMENTATION >>> -----------------------------------------------------------------------

1. The entire screen never gets updated during a game, only 2 squares will be drawn, 
   one for the new head of the snake (the next movement), and one to hide the tail
   of the snake to maintain the proper length. The 4x4 square that hides the tail
   has a lighter color than the background so that it seems like the snake is 
   leaving a trail.

2. The screen is split in a 60x40 grid, and each cell will keep record in what 
   direction the snake was moving so that the code knows which segment of the 
   snake to delete next.

3. When a target gets eaten, the game won't hide the tail for the next screen update
   so that the snake will increase in length. After the target get eaten a new target 
   location is picked randomly (any location that is within the red borders and not 
   on the snake body is valid).

4. To check whenever the snake is eating itself, a 60x40 matrix is used to keep track
   of all the segments of the body (the value is set to 1 if in that cell there is a 
   segment of the snake). A matrix is used because it is more efficient to check one 
   single cell at a (x, y) location instead than looping through a 2400 array at every 
   new frame.








