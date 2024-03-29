# Super Mario Clone
Welcome to Super Mario Adventure! This is a classic side scroller game inspired by the iconic Super Mario series.

## Description
Super Mario Adventure is a 2D side-scrolling platform game where players control the legendary plumber, Mario, as he embarks on an exciting journey to rescue Princess Peach from the clutches of the evil Bowser.

## Features
- keyboard control of L/R/jump movement
- automatic side scrolling
- interactive boundaries & walls
- enemies

## Controls
- To move left - press the left arrow key
- To move right - press the right arrow key
- To jump - press the space bar

These keys can be pressed together to further diversify player movement.

## How to install
1. Download the zip file and unzip it to a location of your choosing
2. Download SFML and ensure it is installed in the correct directory. (for linux use `sudo apt-get install libsfml-dev`)

## How to run
1. Open Ubuntu or an equivalent Linux system.
2. Navigate to the SuperMarioClone folder in the repository.
3. Type: `make`
4. Type: `./SuperMarioClone`
5. Press `Q` to restart the level, or navaigate to the window controller on the top right of the screen to close or resize the window.

## How to play
The goal of the game is to traverse the level, bypass enemies, and collect as many coins and powerups as possible, all while minimizing the time to complete the level.
Coins and powerups offer advantages in the game and allow the player to gain lives or gain special abilities. 
Enemies can be eliminated by jumping on their heads. Collision with an enemy will result in a player losing a life.
A player will lose a life when they fall out of bounds, and the level will be restarted.
