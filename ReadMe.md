# FRTEngine

![game screenshot](ReadmeSrc/screenshot.jpg)

### Controls

`ESC` - exit the game

#### Tetromino

`arrow up` - instantly drop tetromino<br>
`other arrows` - move tetromino<br>
`Z`/`X` - rotate tetromino counterclockwise/clockwise<br>

#### Camera

`W`, `A`, `S`, `D` - move<br>
`T`, `F`, `G`, `H` - change angle of view<br>
`I` - reset position and rotation

### Gameplay
There are seven types of tetrominoes that are randomly chosen and spawned when needed.<br>
They fall by themselves every several moments (depending on a level).<br>
You can rotate a falling tetromino clockwise or counterclockwise, accelerate its falling speed or instantly drop.<br>
If one of the board lines is full-filled by blocks (parts of tetromino), it disappears.<br>
If such a case occurs, each block from above is moved down precisely by the amount of cleared lines (gravity does not apply).<br>
There are ten levels in the game. The color of tetrominoes indicates the change of level.<br>
Each subsequent level makes tetromino falling faster.<br>
To reach the next level, three lines (a small number is chosen to simplify testing) should be cleared.<br>
Progress bar of level progression is built in board walls.

### Technical requirements
| Technology | Version |
|:-----------|:-------:|
| DirectX    |    12   |
| Visual C++ | 14.7 or higher |

