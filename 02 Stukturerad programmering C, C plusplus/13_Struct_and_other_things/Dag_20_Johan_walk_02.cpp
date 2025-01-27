//
//  Dag_20_Johan_The_300_lines_of_code_samle_to_digest__walk_02.cpp
//  Dag_20_Johan_The_300_lines_of_code_samle_to_digest__walk_02
//
//  Created by Daniel Eftodi on 2021-11-01.
//

#include "Dag_20_Johan_walk_02.h"

/* INIT */
void Walk_Main(void) {

/*  GOT WARNING: Implicit conversion loses integer precision: 'time_t' (aka 'long') to 'unsigned int'
 
    FIXED: with casting (unsigned int)
*/  
    std::srand( (unsigned int) std::time(0) );

    // Treasure position is decided randomly.
    Treasure_t treasure =
    {
      .position = {.xPosition = std::rand() % Board_t.xDimension,
                   .yPosition = std::rand() % Board_t.yDimension}
    };

    // Traps are placed around the map. It is not guaranteed
    // that traps position doesn't converge.
    // In that case, the second trap can be assumed to not exist.
    Trap_t trapsInMap[NUMBEROFTRAPS]; //Här skulle man kunna lägga till kod som fixar detta.
    
    /*LOOP*/
    for (int i = 0; i < NUMBEROFTRAPS; i++)
    {
        int xPos = std::rand() % Board_t.xDimension;
        int yPos = std::rand() % Board_t.yDimension;
        
        Trap_t trap =
        {
            .position = {.xPosition = xPos, .yPosition = yPos}
        };
        
        trapsInMap[i] = trap;
    }

    // Bandits are placed around the map. It is not guaranteed
    // that bandits position doesn't converge, but they will move
    // anyway.
    Bandit_t banditsInMap[NUMBEROFBANDITS];
    
    /*LOOP*/
    for (int i = 0; i < NUMBEROFBANDITS; i++)
    {
        int xPos = std::rand() % Board_t.xDimension;
        int yPos = std::rand() % Board_t.yDimension;
        
        Bandit_t bandit =
        {
            .position = {.xPosition = xPos, .yPosition = yPos}
        };
        
        banditsInMap[i] = bandit;
    }

    // Player position on the 1st turn is randomly decided.
    // It can not be the same of a bandit or a trap.
    bool match = false;
    int xPos;
    int yPos;
    
    /*LOOP*/
    do
    {
        xPos = std::rand() % Board_t.xDimension;
        yPos = std::rand() % Board_t.yDimension;
        
        for (int i = 0; i < NUMBEROFTRAPS; i++)
        {
          if (( (xPos == trapsInMap[i].position.xPosition) &&
                (yPos == trapsInMap[i].position.yPosition) )||
              
               ((xPos == banditsInMap[i].position.xPosition) &&
                (yPos == banditsInMap[i].position.yPosition))
              )
          {
            match = true;
          }
        }
        
    } while (match);

    // Player is placed on the map.
    Player_t player =
    {
        .position = {.xPosition = xPos, .yPosition = yPos}
        
    };

    // The order of the turn is the following:
    // 1. Board is drawn.
    // 2. User is asked for movement direction.
    // 3. Player moves in the chosen direction.
    // 4. Bandits move.
    int maxTurnos = INT_MAX;
    
    /*LOOP*/
    for (int i = 0; i <= maxTurnos; i++)
    {
        // 1. Board is drawn.
        drawBoard2(player, trapsInMap, banditsInMap, treasure);
        
        // 2. User is asked for movement direction.
        Direction direction;
    
        /*LOOP - NESTED*/
        do
        {
            direction = askDirection();
//            printf("\n");
        }
        while (direction == WRONG_DIRECTION);
        
        // 3. Player moves in the chosen direction.
        movePlayer(player, direction);
//        moveCharacter(worldDungeon.Player.symbol, worldDungeon, direction);
        
        // 4. Bandits move.
        /*LOOP - NESTED*/
        for (int j = 0; j < NUMBEROFBANDITS; j++)
        {
            moveBandit(banditsInMap[j]);
//            moveCharacter(worldDungeon.Bandit.symbol, worldDungeon);
        }
                
//        // Reset the terminal
//        clearScreen();
    }
    
}

void drawBoard(Player_t   player,
               Trap_t     totalTraps[],
               Bandit_t   totalBandits[],
               Treasure_t treasure)
{
    bool squareDrawn = false;

    /*LOOP*/
    for (int y = 0; y <= Board_t.yDimension; y++)
    {
        /*LOOP - NESTED*/
        for (int x = 0; x <= Board_t.xDimension; x++)
        {
            /*LOOP - NESTED*/
            // Traps are drawn
            for (int z = 0; z <= NUMBEROFTRAPS; z++) {
                Trap_t trapToDraw = totalTraps[z];
                
                if ( (trapToDraw.position.xPosition == x) &&
                     (trapToDraw.position.yPosition == y) )
                {
                    printf("%c", trapToDraw.symbol);
                    squareDrawn = true;
                }
            }
            
            /*LOOP - NESTED*/
            // Bandits are drawn.
            // In case of collision with a trap,
            // only the second is drawn.
            for (int z = 0; z <= NUMBEROFBANDITS; z++)
            {
                Bandit_t banditToDraw = totalBandits[z];
                if ( (banditToDraw.position.xPosition == x) &&
                     ((banditToDraw.position.yPosition == y) && (!squareDrawn)) )
                {
                    printf("%c", banditToDraw.symbol);
                    squareDrawn = true;
                }
            }

            // Treasure is drawn. If position of treasure == position of player
            // game ends with victory
            if ( (x == treasure.position.xPosition) &&
                 (y == treasure.position.yPosition) )
            {
                if ( (treasure.position.xPosition == player.position.xPosition) &&
                     (treasure.position.yPosition == player.position.yPosition) )
                {
                    endGame(VICTORY);
                }

                printf("X");
                continue;
            }

            if ( (x == player.position.xPosition) && (y == player.position.yPosition) )
            {
                if (squareDrawn) { endGame(DEFEAT); }
                
                printf("P");
                continue;
            }
            
            // Empty square "." is drawn. It only gets printed if there is nothing
            // on the square.
            if (!squareDrawn) { printf("."); }
            
            squareDrawn = false;
        }
        
        printf("\n");
    }
}

void drawBoard2(Player_t   player,
                Trap_t     totalTraps[],
                Bandit_t   totalBandits[],
                Treasure_t treasure)
{
    bool squareDrawn = false;
    
    /*LOOP*/
    for (int y = 0; y <= Board_t.yDimension; y++)
    {
        /*LOOP - NESTED*/
        for (int x = 0; x <= Board_t.xDimension; x++)
        {
            /*LOOP - NESTED*/
            // Traps are drawn
            for (int z = 0; z <= NUMBEROFTRAPS; z++) {
                Trap_t trapToDraw = totalTraps[z];
                
                if ( (trapToDraw.position.xPosition == x) &&
                    (trapToDraw.position.yPosition == y) )
                {
                    currentToRender[x][y] = trapToDraw.symbol;
                    squareDrawn = true;
                }
            }
            
            /*LOOP - NESTED*/
            // Bandits are drawn.
            // In case of collision with a trap,
            // only the second is drawn.
            for (int z = 0; z <= NUMBEROFBANDITS; z++)
            {
                Bandit_t banditToDraw = totalBandits[z];
                if ( (banditToDraw.position.xPosition == x) &&
                    ((banditToDraw.position.yPosition == y) && (!squareDrawn)) )
                {
                    currentToRender[x][y] = banditToDraw.symbol;
                    squareDrawn = true;
                }
            }
            
            // Treasure is drawn. If position of treasure == position of player
            // game ends with victory
            if ( (x == treasure.position.xPosition) &&
                (y == treasure.position.yPosition) )
            {
                if ( (treasure.position.xPosition == player.position.xPosition) &&
                    (treasure.position.yPosition == player.position.yPosition) )
                {
                    endGame(VICTORY);
                }
                
                currentToRender[x][y] = 'X';
                continue;
            }
            
            if ( (x == player.position.xPosition) && (y == player.position.yPosition) )
            {
                if (squareDrawn) { endGame(DEFEAT); }
                
                currentToRender[x][y] = 'P';
                continue;
            }
            
            // Empty square "." is drawn. It only gets printed if there is nothing
            // on the square.
            if (!squareDrawn) { currentToRender[x][y] = '.'; }
            
            squareDrawn = false;
        }
    }
    
    // clear the screen before drawing
    clearScreen();
    
    /*LOOP*/
    for (int y = 0; y <= Board_t.yDimension; y++)
    {
   
#if DEBUG_LOGGING
        //  DEBUG LOGG ARRAY NUMBERS
        if (y < 10)
        {
            printf("[0%d] ", y);
        }
        else
        {
            printf("[%d] ", y);
        }
#endif
        
        /*LOOP - NESTED*/
        for (int x = 0; x <= Board_t.xDimension; x++)
        {

#if DEBUG_LOGGING
//  DEBUG LOGG ARRAY NUMBERS
            if (x < 10)
            {
                printf("[0%d]", x);
            }
            else
            {
                printf("[%d]", x);
            }
#endif

            // print buffer
            printf("%c ", currentToRender[x][y]);
            
            // clear buffer
            currentToRender[x][y] = '.';
        }
        
        // new-line after each row ...
        printf("\n");
    }
}

Direction askDirection()
{
#if DEBUG_LOGGING
    printf("askDirection() :: IN  :: %d\n", ++DBG_Counter);
#endif
    
    printf("Select [L/A]eft, [R/D]ight, [T/W]op or [B/S]ottom: ");
    char answer;
    std::cin >> answer;

    Direction chosenDirection;
    switch (std::toupper(answer))
    {
        case 'L':
        case 'A':
            chosenDirection = LEFT;
            break;
        case 'R':
        case 'D':
            chosenDirection = RIGHT;
            break;
        case 'T':
        case 'W':
            chosenDirection = TOP;
            break;
        case 'B':
        case 'S':
            chosenDirection = BOTTOM;
            break;
        default:
            chosenDirection = WRONG_DIRECTION;
            break;
    }
    
#if DEBUG_LOGGING
    printf("askDirection() :: OUT :: %d\n", DBG_Counter);
    delay(1.5);
#endif
    
    return chosenDirection;
}

void movePlayer(Player_t &player,
                Direction direction)
{
    switch (direction) {
        case RIGHT:
            if (player.position.xPosition < Board_t.xDimension)
              player.position.xPosition += 1;
            break;
        case LEFT:
            if (player.position.xPosition > 0)
              player.position.xPosition -= 1;
            break;
        case TOP:
            if (player.position.yPosition > 0)
              player.position.yPosition -= 1;
            break;
        case BOTTOM:
            if (player.position.yPosition < Board_t.yDimension)
              player.position.yPosition += 1;
            break;
        case WRONG_DIRECTION:
#if !DEBUG_LOGGING
            printf("\nmovePlayer :: **Incorrect input**\n");
#endif
            
            break;
        default:
            break;
    }
}

void moveBandit(Bandit_t &bandit)
{
    int direction = std::rand() % 4;
    
    switch (direction) {
        case 0:
            if (bandit.position.xPosition < Board_t.xDimension)
                bandit.position.xPosition += 1;
            break;
        case 1:
            if (bandit.position.xPosition > 0)
                bandit.position.xPosition -= 1;
            break;
        case 2:
            if (bandit.position.yPosition > 0)
                bandit.position.yPosition -= 1;
            break;
        case 3:
            if (bandit.position.yPosition < Board_t.yDimension)
                bandit.position.yPosition += 1;
            break;
        default:
            break;
    }
}

void moveCharacter(char cCharacter,
                   World &world,
                   Direction direction)
{
    
    int iBandit_direction = std::rand() % 4;
    
    switch (cCharacter) {
        case 'B':
            switch (iBandit_direction) {
                case 0:
                    if (world.tBandit.position.xPosition < Board_t.xDimension)
                        world.tBandit.position.xPosition += 1;
                    break;
                case 1:
                    if (world.tBandit.position.xPosition > 0)
                        world.tBandit.position.xPosition -= 1;
                    break;
                case 2:
                    if (world.tBandit.position.yPosition > 0)
                        world.tBandit.position.yPosition -= 1;
                    break;
                case 3:
                    if (world.tBandit.position.yPosition < Board_t.yDimension)
                        world.tBandit.position.yPosition += 1;
                    break;
                default:
                    break;
            }
            printf("Bandit :: Directon: %d\n", iBandit_direction);
            break;
        case 'P':
            switch (direction) {
                case RIGHT:
                    if (world.tPlayer.position.xPosition < Board_t.xDimension)
                        world.tPlayer.position.xPosition += 1;
                    break;
                case LEFT:
                    if (world.tPlayer.position.xPosition > 0)
                        world.tPlayer.position.xPosition -= 1;
                    break;
                case TOP:
                    if (world.tPlayer.position.yPosition > 0)
                        world.tPlayer.position.yPosition -= 1;
                    break;
                case BOTTOM:
                    if (world.tPlayer.position.yPosition < Board_t.yDimension)
                        world.tPlayer.position.yPosition += 1;
                    break;
                case WRONG_DIRECTION:
#if !DEBUG_LOGGING
                    printf("\nmoveCharacter :: **Incorrect input**\n");
#endif
                    break;
                default:
                    break;
            }
            printf("Player :: Directon: %d\n", direction);
        default:
            break;
    }
}

void endGame(Result result)
{
    std::string announcement = (result == VICTORY) ? "YOU WIN" : "GAME OVER";
    
    // Reset the terminal
    clearScreen();
    
    printf("\n\n");
    printf("===========================\n");
    printf("||\t\t\t ||\n");
    if ( result == VICTORY )
    {
        printf("||\t%s\t\t ||\n", announcement.c_str());
    }
    else
    {
        printf("||\t%s\t ||\n", announcement.c_str());
    }
    printf("||\t\t\t ||\n");
    printf("===========================\n");
    exit(1);
}

void clearScreen(void)
{
    #ifdef __WIN32__
        system("cls");
    #else
        system("clear");
    #endif
}

void delay(int iDelayTime)
{
#ifdef __WIN32__
    Sleep(iDelayTime);
#else
    sleep(iDelayTime);
#endif
}
