                   _                                  _     _______            
     /\           (_)                                | |   |__   __|           
    /  \   ___ ___ _  __ _ _ __  _ __ ___   ___ _ __ | |_     | |_      _____  
   / /\ \ / __/ __| |/ _` | '_ \| '_ ` _ \ / _ \ '_ \| __|    | \ \ /\ / / _ \ 
  / ____ \\__ \__ \ | (_| | | | | | | | | |  __/ | | | |_     | |\ V  V / (_) |
 /_/    \_\___/___/_|\__, |_| |_|_| |_| |_|\___|_| |_|\__|    |_| \_/\_/ \___/ 
                      __/ |                                                    
                     |___/                                                     
================================================================================

Joseph Bess
John Herrick
Laura Shub

Included in the zip is our resource tree that has every file necessary to build and run our game.

We included a prebuilt executable so that you can just run it out of the box, no building necessary. This executable is called 'assignment2' and typing `./assignment2` from the root directory will launch the executable.

If you want to build our project from source, in the root directory type `./buildit` then `./makeit` and then launch the newly created executable with `./assignment2`.

Make sure to read report.pdf and manual.pdf.


  ______      _                _____              _ _ _   
 |  ____|    | |              / ____|            | (_) |  
 | |__  __  _| |_ _ __ __ _  | |     _ __ ___  __| |_| |_ 
 |  __| \ \/ / __| '__/ _` | | |    | '__/ _ \/ _` | | __|
 | |____ >  <| |_| | | (_| | | |____| | |  __/ (_| | | |_ 
 |______/_/\_\\__|_|  \__,_|  \_____|_|  \___|\__,_|_|\__|
                                                          
===========================================================

We also touch on extra credit in the User Manual but we wanted to include it here as well. We are happy to say we completed every optional extra credit piece.

Improved Camera Controls (1 pt)
	- The main focus in Pong is controlling the paddle and the user shouldn't have to worry about controlling the camera. Because of this we decided to implement a camera system based around the movement of the paddle so that any time the user moves the paddle the camera intuitively follows. Additionally, when the user pauses the game with `esc`, the WASD EQ keys allow the user to move the camera in a global view throughout the game board. When the user resumes, the camera snaps back to the intuitive mode.

Background Music (2pts)
	- We have included background music in our game. When the user pauses, an option pops up to control the volume of specifically the background music.

End Condition (2 pts)
	- The game ends when one side reaches 7 points. Once that condition is met, the player is presented with an end of game screen displaying the score. If the player wins fireworks play as well as a victory sound.

Particle Effects (2pt)
	- We implemented particle effects and any time the player or opponent scores, the ball explodes into a bunch of colorful particles. Additionally, when the player wins we play fireworks in the background, built using a series of particles and particle emmitters.

Player Interfaces (3 pts)
	- Upon launching the game the user is presented with a start button which functions as the Main Menu. After clicking start, the game begins and the user can pause with `esc`. After pausing, the physics stop and the user has the option to change sound options, continue the game, and quit the game.
