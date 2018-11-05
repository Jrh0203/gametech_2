                   _                                  _     _______ _                   
     /\           (_)                                | |   |__   __| |                  
    /  \   ___ ___ _  __ _ _ __  _ __ ___   ___ _ __ | |_     | |  | |__  _ __ ___  ___ 
   / /\ \ / __/ __| |/ _` | '_ \| '_ ` _ \ / _ \ '_ \| __|    | |  | '_ \| '__/ _ \/ _ \
  / ____ \\__ \__ \ | (_| | | | | | | | | |  __/ | | | |_     | |  | | | | | |  __/  __/
 /_/    \_\___/___/_|\__, |_| |_|_| |_| |_|\___|_| |_|\__|    |_|  |_| |_|_|  \___|\___|
                      __/ |                                                             
                     |___/                                                              
=========================================================================================

Joseph Bess
John Herrick
Laura Shub

Included in the zip is our resource tree that has every file necessary to build and run our game.

We included a prebuilt executable so that you can just run it out of the box, no building necessary. This executable is called 'assignment2' and typing `./assignment2` from the root directory will launch the executable. We know we're on assignment 3 but the executable is still named assignment2.

If you want to build our project from source, in the root directory type `./buildit` then `./makeit` and then launch the newly created executable with `./assignment2`.

Make sure to read report.pdf and manual.pdf.


  _   _      _                      _    _             
 | \ | |    | |                    | |  (_)            
 |  \| | ___| |___      _____  _ __| | ___ _ __   __ _ 
 | . ` |/ _ \ __\ \ /\ / / _ \| '__| |/ / | '_ \ / _` |
 | |\  |  __/ |_ \ V  V / (_) | |  |   <| | | | | (_| |
 |_| \_|\___|\__| \_/\_/ \___/|_|  |_|\_\_|_| |_|\__, |
                                                  __/ |
                                                 |___/ 
===========================================================

We touched on it in the user manual but this is a distilled tl;dr version of how to use the networking component of our game

Upon loading the executable you have the option to host or join. Have one executable host a game and the other act as the client and join the game. Enter the public IP address of the host and press enter to connect. It does not matter if you host or join first, they will both wait for each other. The host is in charge of the game and they control when the ball starts by pressing space. They are also the only one allowed to pause the game. The game functions completely normally except now there are two players and for controls reference the user manual.