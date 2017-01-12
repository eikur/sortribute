# Streets Of Rage Tribute Page

This game tribute is a project developed for the UPC School's Design and Creation of Videogames master 2016-2017.

It features one of my favourite levels of the original Streets of Rage, Moonbeach, and while it misses the enemy variety, it has the enemy and item placement spot on.

Github: [https://github.com/eikur/sortribute](https://github.com/eikur/sortribute)   

Download release version [here](https://github.com/eikur/sortribute/files/699931/Streets.Of.Rage.Tribute.release.zip)   

Download source code [here](https://github.com/eikur/sortribute/archive/1.0.zip)   

License: GNU GPL version 2  
Author: Jorge Soriano Pinedo // eikur  

Gameplay video: 
<iframe width="560" height="315" src="https://www.youtube.com/embed/GutNYy9ybMo" frameborder="0" allowfullscreen></iframe>
If the video doesn't work, try [here](https://youtu.be/GutNYy9ybMo)   


## Executing the game
Extract all the zip contents and simply double click in the 'SoR Tribute.exe' file. The rest is just playing!  
_Known issues_: The release has been compiled in Visual Studio 2015 under Windows 10, and it does not work in computers with Windows 7 without Visual Studio packages installed.

## Game Goal
The game's objective is to make your way through swarms of enemies defeating them. In the end you will find the boss of the zone, which is consistently stronger and more difficult.If you can make it to the boss and you defeated you win, and if you can't (and lose all of your lives in the process) it's game over.

## Controls

### Player Controls

The player actions have been maped following the original Genesis gamepad, which had the crosshair for movement, Start button for pause and 3 action buttons.  
The mapping is the following:  

- **Up** / **Down** / **Left** / **Right** : Move the character around
- **ENTER**: Pause button  
- **Z**: Special button. Consumes one special point. Unfortunatelly it has no visual animation and only a strong sound when activated. That, and the enemies become anihilated.  
- **X**: Attack button. If performed consecutively when hitting an enemy will create a combo that ends with a high kick.
- **C**: Jump button

Special cases:

- Walking to an enemy without being hit will result in a player hold, either frontal or backwards. To disengage and free the enemy, move in the opposite direction than the hold.  
- **C (when holding an enemy)**: switches between front and back hold. This switch can only be performed once successfully, the second time the player will leave the hold after the position switch.  
- **X (when back holding)**: German suplex for the lucky enemy you are holding  
- **X (when front holding)**: Hold hit, which can be combined to perform a hold combo (knee hit + knee hit + headbutt)  
- **X + opposite direction than the hold (when front holding)**: Throw an enemy over your head. You can knock down the enemies that come your way if it impacts them.  
- **X + C (at the same time)**: Back attack. Unfortunately, it only has been animated and has no real effect on enemies in this tribute.

### Debug Controls

The debug features used during development have been kept to play with them in case you feel like it:  

- **_F1_** : Debug Colliders  
It will show the colliders set in the scene, player, enemies and items. Pressing it again will hide them.  
- **_1_, _2_, _3_ and _4_**: Change gameplay variables  
Add more lives, score and special attacks respectively by pressing the corresponding key. Be careful, though, because key 4 subtracts time to your gameplay timer!  
- **Q, W**: Test player reactions  
Use them to hit and knock the player respectively.  
- **E and R**: Spawn enemies  
Use E to spawn cannon fodder enemies and use R to spawn bosses(!!)
- **A and S**: Spawn health items  
Use A to spawn an apple (25% health recovery) or S for a chicken (100% recovery)  
- **O, P and L**: Camera behaviour  
Use O to move the camera left, use P to move it right and use L to shift between locked and unlocked status. Be extra careful with these debug options, **a lot** of systems depend on the camera position and locking for their proper functioning.

## After playing the game
You can play multiple times in a row, without the program exiting or crashing on you.  


## Acknowledgements
This projects needed a good JSON parser, and my election was the work of Krzysztof Gabis, parson. It worked like a charm, almost plug and play, and this release would not have been possible without it. You can find it [here](https://github.com/kgabis/parson)  
Many thanks to the Streets of Rage online fanbase, for all the visual and audio assets I could find in almost no time.  
Last but not least, thanks to my teachers for the support, my girlfriend for the countless hours she has been patient, and the countless testers that offered their services just to help. I owe you all a beer.  

I hope you enjoyed playing this game as much as I enjoyed creating it.  
Jordi
