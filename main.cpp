#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHBuzzer.h>
#include <cstdlib>
#include <time.h>

using namespace std;

// Defines constants for the number of obstacles and coins to display during each round
#define NUM_OBSTACLES 10
#define NUM_COINS 20
#define MAX_SPEED 5

// Declares Function Prototypes
int playGame();
void displayMenu();
void displaySplashScreen();
void displayCountdown();
void createGoBack();

// Defines a Kart class for the player during the game
// REQUIRED CLASS/OBJECT
class Kart{

public:
    // Declares all the functions to be used with the Kart Object
    Kart(int cartColor);
    void increaseSpeed();
    void resetSpeed();
    int getSpeed();
    int getXCoord();
    int getYCoord();
    void adjustYCoordUp();
    void adjustYCoordDown();
    int getColor();

private:
    // Declares the object attributes
    int color;
    int speed;
    int xcoord;
    int ycoord;

};

// The constructor for the Kart. Takes in a value for color from user
Kart::Kart(int cartColor){

    // Sets user-chosen color to the appropriate attribute
    color = cartColor;

    // Sets default initial values to object attributes
    speed = 1;
    xcoord = 5;
    ycoord = 100;
}

// Function increases the speed attribute of the Kart
void Kart::increaseSpeed(){

    // Increases speed as long as it hasn't reached the Max Speed Value
    if(speed < MAX_SPEED){
        speed++;
    }
}

// Function resets the object's speed to the minimum value
void Kart::resetSpeed(){
    speed = 1;
}

// Functions provides the x-coordinate of the top-left corner of the object
int Kart::getXCoord(){
    return xcoord;
}

// Functions provides the y-coordinate of the top-left corner of the object
int Kart::getYCoord(){
    return ycoord;
}

// Function provides the current speed of the object
int Kart::getSpeed(){
    return speed;
}

// Function adjusts the y-coordinate of the top-left corner of the object to move towards the origin
void Kart::adjustYCoordDown(){

    // Checks to make sure the top left edge of the object won't go past the edge of the screen
    if(ycoord > 20){
        ycoord -= 20;
    }
}

// Function adjusts the y-coordinate of the top-left corner of the object to move away from the origin
void Kart::adjustYCoordUp(){

    // Checks to make sure the top left edge of the object won't go past the edge of the screen
    if(ycoord < 168){
        ycoord += 20;
    }
}

// Function returns the value of the color property of the object
int Kart::getColor(){
    return color;
}

// Function is responsible for facilitating going back to the main menu for the Play, Statistics, Credits, and Help sections of the Main Menu options
// REQUIRED USER-DEFINED FUNCTION
void createGoBack(){

    // Declares floats for holding values of the last touch
    float x, y;

    // Displays instructions
    LCD.SetFontColor(BLUE);
    LCD.WriteLine("Tap anywhere to return to the Menu");

    // Declares and initializes the loop control variable
    bool goBack = true;

    // Checks for touches on the screen until the button is touched
    // REQUIRED WHILE LOOP
    while (goBack) {

        // Checks if the touch was registered
        if (LCD.Touch(&x, &y)) {

            // Negates the loop control variable to exit loop
            goBack = false;
        }
    }
}


// Function displays the menu and controls all actions
void displayMenu() {

    // Initializes the variables to hold the top scores and the most recent score
    int topScores[9];
    int lastScore = 0;

    // Declare an array of four icons called menu (FEH Proteus Documentation)
    FEHIcon::Icon menu[4];

    // Define the five menu labels
    char menu_labels[4][20] = {"PLAY", "SEE STATISTICS", "GET HELP", "CREDITS"};

    // Declare variables that store x and y coordinates of touch
    float x, y;

    // Loops through the top scores array and initializes each element to 0
    // REQUIRED FOR LOOP
    for(int i = 0; i < 9; i++){
        topScores[i] = 0;
    }

    // Loops through the menu and gameplay process infinitely
    while(true){

        // Clears the screen and sets the background and font color before drawing anything to the screen
        LCD.Clear(FEHLCD::White);
        LCD.SetBackgroundColor(LIGHTSKYBLUE);
        LCD.SetFontColor(BLACK);

        // Draw the menu in a 4 by 1 array with top and bottom
        // margins of 10 and left and right margins of 5
        // with the menu labels, white borders, and white text (FEH Proteus Documentation)
        FEHIcon::DrawIconArray(menu, 4, 1, 10, 10, 5, 5, menu_labels, BLACK, BLACK);

        // Declares and initializes the loop control variable for checking for touches on the menu
        bool isMenuActive = true;

        // Loops through while still viewing the main menu
        while (isMenuActive) {

            // Store coordinates of touch into x and y
            LCD.Touch(&x, &y);

            // If Play button is pressed,
            // REQUIRED IF-ELSE TREE
            if (menu[0].Pressed(x, y, 0)) {

                // Button border is emphasized
                menu[0].Select();

                // Clears screen and displays loading message for the player for two seconds
                LCD.Clear();
                LCD.WriteLine("Have fun playing!");
                LCD.WriteLine("We're loading your game...");
                Sleep(2.0);

                // Calls the function to play the game and saves the score returned after playing
                lastScore = playGame();

                // Negates the loop control variable to exit the loop
                isMenuActive = false;

                // Clears the screen and sets screen background and font color
                LCD.Clear(BLACK);
                LCD.SetBackgroundColor(BLACK);
                LCD.SetFontColor(WHITE);

                // Display end-of-game message and round score
                LCD.WriteLine("Thanks for playing!");
                LCD.WriteAt("Your overall score is: ", 30, 100);
                LCD.WriteAt(lastScore, 90, 120);

                Sleep(3.0);

                // Allows ability to go back to main menu
                createGoBack();

                // Adds the most recent score to the leaderboard and sorts the leaderboard array
                for(int s = 0; s < 9; s++){
                    if(lastScore > topScores[s]){
                        int temp = topScores[s];
                        topScores[s] = lastScore;
                        lastScore = temp;
                    }
                }
            }

            // If See Statistics button is pressed
            else if (menu[1].Pressed(x, y, 0)) {

                // Emphasizes the button borders
                menu[1].Select();

                // Clears the screen and writes the screen title
                LCD.Clear();
                LCD.WriteLine("Overall statistics: ");

                // Loops through the high scores array and prints them to the screen
                for(int score = 0; score < 9; score++){
                    LCD.Write(score+1);
                    LCD.Write(")  ");
                    LCD.WriteLine(topScores[score]);
                }

                // Calls funtion to add the ability to go back to the menu screen upon the players request
                createGoBack();

                // Negates the loop control variable to allow the menu function to exit
                isMenuActive = false;

            }

            // If Get Help button is pressed
            else if (menu[2].Pressed(x, y, 0)) {

                // Emphasizes the button borders
                menu[2].Select();

                // Clears the screen and writes the screen title
                LCD.Clear();
                LCD.WriteLine("Rules of the game: ");
                LCD.WriteLine("");
                LCD.WriteLine("1) Hold the side of the");
                LCD.WriteLine("screen in which you");
                LCD.WriteLine("want the car to move.");
                LCD.WriteLine("2) Avoid the black");
                LCD.WriteLine("squares. They slow you");
                LCD.WriteLine("down!");
                LCD.WriteLine("3) Catch the yellow coins!");
                LCD.WriteLine("4) Finish the game as fast");
                LCD.WriteLine("as you can!");

                // Calls funtion to add the ability to go back to the menu screen upon the players request
                createGoBack();

                // Negates the loop control variable to allow the menu function to exit
                isMenuActive = false;
            }

            // If Credits button is pressed
            else if (menu[3].Pressed(x, y, 0)) {

                // Emphasizes the button borders
                menu[3].Select();

                // Clears the screen and writes the screen content
                LCD.Clear();
                LCD.WriteLine("This source code was");
                LCD.WriteLine("written by Jasmohan Bawa");
                LCD.WriteLine("and Apoorva Mahesh in FEH.");
                LCD.WriteLine("Sources consulted include");
                LCD.WriteLine("Professor Richard Freuler,");
                LCD.WriteLine("GTA Drew Phillips, and");
                LCD.WriteLine("UTAs Eben Carek and");
                LCD.WriteLine("Namrata Arya. Thank you");
                LCD.WriteLine("for your guidance!");

                // Calls funtion to add the ability to go back to the menu screen upon the players request
                createGoBack();

                // Negates the loop control variable to allow the menu function to exit
                isMenuActive = false;
            }
        }
    }
}

// Function displays the splash screen of the game
void displaySplashScreen() {

    // Clears the screen
    LCD.Clear(LIGHTSKYBLUE);

    // Set Background Color of screen to light sky blue
    LCD.SetBackgroundColor(LIGHTSKYBLUE);

    // Set Font Color on screen to tomato
    LCD.SetFontColor(TOMATO);

    // Print ASCII-style block text
    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("      ______________  __ ");
    LCD.WriteLine("     / ____/ ____/ / / / ");
    LCD.WriteLine("    / /_  / __/ / /_/ /  ");
    LCD.WriteLine("   / __/ / /___/ __  /   ");
    LCD.WriteLine("  /_/ __/_____/_/ /_/ __ ");
    LCD.WriteLine("     / //_/___ ______/ /_");
    LCD.WriteLine("    / ,< / __ `/ ___/ __/");
    LCD.WriteLine("   / /| / /_/ / /  / /_  ");
    LCD.WriteLine("  /_/ |_\__,_/_/   \__/  ");

    // Our awesome theme song plays on the device
    Buzzer.Tone(FEHBuzzer::C3, 200);
    Buzzer.Tone(FEHBuzzer::E3, 200);
    Buzzer.Tone(FEHBuzzer::G3, 200);
    Buzzer.Tone(FEHBuzzer::B3, 200);
    Buzzer.Tone(FEHBuzzer::C4, 500);

    // Leave on screen for 3 seconds
    Sleep(3.0);

}

// Function displays ASCII Art Countdown
void displayCountdown(){

    LCD.Clear(BLACK);
    LCD.SetBackgroundColor(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("_____/\\\\\\\\\\__       ");
    LCD.WriteLine(" ___/\\\///////\\\_      ");
    LCD.WriteLine("  __\///______/\\\__     ");
    LCD.WriteLine("   _________/\\\//___    ");
    LCD.WriteLine("    ________\////\\\__   ");
    LCD.WriteLine("     ___________\//\\\_  ");
    LCD.WriteLine("      __/\\\______/\\\__ ");
    LCD.WriteLine("       _\///\\\\\\\\\/___");
    LCD.WriteLine("        ___\/////////____");

    // Leave on screen for 1 seconds
    Sleep(1.0);

    LCD.Clear(BLACK);
    LCD.SetBackgroundColor(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("____/\\\\\\\\\_____      ");
    LCD.WriteLine(" __/\\\///////\\\___     ");
    LCD.WriteLine("  _\///______\//\\\__    ");
    LCD.WriteLine("   ___________/\\\/___   ");
    LCD.WriteLine("    ________/\\\//_____  ");
    LCD.WriteLine("     _____/\\\//________ ");
    LCD.WriteLine("      ___/\\\/___________");
    LCD.WriteLine("      __/\\\\\\\\\\\\\\\_");
    LCD.WriteLine("       _\///////////////_");

    // Leave on screen for 1 seconds
    Sleep(1.0);

    LCD.Clear(BLACK);
    LCD.SetBackgroundColor(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("      ______/\\\_        ");
    LCD.WriteLine("       __/\\\\\\\_       ");
    LCD.WriteLine("        _\/////\\\_      ");
    LCD.WriteLine("         _____\/\\\_     ");
    LCD.WriteLine("          _____\/\\\_    ");
    LCD.WriteLine("           _____\/\\\_   ");
    LCD.WriteLine("            _____\/\\\_  ");
    LCD.WriteLine("             _____\/\\\_ ");
    LCD.WriteLine("              _____\///_ ");

    // Leave on screen for 1 seconds
    Sleep(1.0);

    LCD.Clear(BLACK);
    LCD.SetBackgroundColor(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("");
    LCD.WriteLine("   ___     ___      _    ");
    LCD.WriteLine("  / __|   / _ \    | |   ");
    LCD.WriteLine(" | (_ |  | (_) |   |_|   ");
    LCD.WriteLine("  \___|   \___/   _(_)_  ");
    LCD.WriteLine("_|\"\"\"\"\"|_|\"\"\"\"\"|_| \"\"\" | ");
    LCD.WriteLine("\"`-0-0-'\"`-0-0-'\"`-0-0-' ");

    // Leave on screen for 1/2 a second
    Sleep(.5);
}

// Function responsible for going through with all gameplay
int playGame(){

    /*
     * Create the Character Menu
     */

    // Declare an array of four icons called menu (FEH Proteus Documentation)
    FEHIcon::Icon menu[4];

    // Define the five menu labels
    char menu_labels[4][20] = {"Captain Phillips", "Dr. Rick", "Eben", "Namrata"};

    // Clears the screen, sets the background color, and displays the user prompt
    LCD.Clear( FEHLCD::White );
    LCD.SetFontColor(BLACK);
    LCD.WriteLine("Choose your character:");

    // Draw the menu in a 4 by 1 array with top margins of 20 and bottom
    // margins of 10 and left and right margins of 5
    // with the menu labels, white borders, and white text (FEH Proteus Documentation)
    FEHIcon::DrawIconArray(menu, 4, 1, 20, 10, 5, 5, menu_labels, BLACK, BLACK);

    // Declares variables
    bool isMenuActive = true;
    float x, y;
    int kartColor;

    // Loops through while a character has not been chosen
    while(isMenuActive){

        // Stores values of touch to x and y
        LCD.Touch(&x, &y);

        // Assigns colors based on which character was chosen and negates the loop control to exit loop
        if (menu[0].Pressed(x, y, 0)) {
            kartColor = GREEN;
            isMenuActive = false;
        }else if (menu[1].Pressed(x, y, 0)) {
            kartColor = SCARLET;
            isMenuActive = false;
        } else if (menu[2].Pressed(x, y, 0)) {
            kartColor = ROYALBLUE;
            isMenuActive = false;
        } else if (menu[3].Pressed(x, y, 0)) {
            kartColor = DARKVIOLET;
            isMenuActive = false;
        }
    }

    /*
     * Creates random map objects
     */

    // Creates player object, and array of coordinates for obstacles and coins
    Kart player(kartColor);
    int obstacleCoord[NUM_OBSTACLES][2];
    int coinCoord[NUM_COINS][2];
    int randy;

    srand(TimeNow());

    // Creates random coordinates for obstacles
    for(int i = 0; i < NUM_OBSTACLES; i++){
        randy = rand() % 189;
        obstacleCoord[i][0] = 269;
        obstacleCoord[i][1] = randy+50;
    }

    // Creates random coordinates for coins
    for(int c = 0; c < NUM_COINS; c++){
        randy = rand() % 189;
        coinCoord[c][0] = 269;
        coinCoord[c][1] = randy+50;
    }

    // Pre-game Intro
    displaySplashScreen();
    displayCountdown();

    /*
     * Begins Gameplay
     */

    // Declares variables for use during gameplay
    int obstacleCounter = 0, coinCounter = 0, obstacleArrayIndex = 1, coinArrayIndex = 1;
    int lastSavedTime, startTime;
    int points = 0;
    int obstaclesCompletedCounter = 0;

    // Clears screen and sets background color
    LCD.Clear( MEDIUMSEAGREEN );

    // Initializes variable with the current time to be used in determining speed change
    lastSavedTime = TimeNow();

    // Initialize start time for score calculation
    startTime = TimeNow();

    // Loops through entire course, which is until all obstacles have been released and destroyed
    // REQUIRED DO-WHILE LOOP
    do{

        /*
         * Draws all on screen objects and checks for collision
         */

        // Draws the Player's Kart
        LCD.SetFontColor(player.getColor());
        LCD.FillRectangle(player.getXCoord(), player.getYCoord(), 50, 50);

        // Resets the loop control variable
        obstaclesCompletedCounter = 0;

        // Loops through obstacles and displays only the number of obstacles that should be displayed
        LCD.SetFontColor(FEHLCD::Black);
        for(int j = 0; j < obstacleArrayIndex; j++){

            // Makes sure obstacle is still in field of play and not been hit yet
            // REQUIRED RELATIONAL AND LOGICAL OPERATORS
            if(obstacleCoord[j][0] > 20 && obstacleCoord[j][0] != 225){

                // Draws Obstacle
                LCD.FillRectangle(obstacleCoord[j][0], obstacleCoord[j][1], 50, 50);

                // Checks if the obstacle and kart are within a certain range to constitute a collision
                if((obstacleCoord[j][0] - player.getXCoord() < 50) && (abs(obstacleCoord[j][1] - player.getYCoord()) < 50)){

                    // Resets the speed time variable to the current time and resets the speed to the slowest
                    lastSavedTime = TimeNow();
                    player.resetSpeed();

                    // Plays obstacle sound effect
                    Buzzer.Tone(250, 300);

                    // 'Deletes' obstacle by blending it with the background and changing the coordinates to the 'dead zone'
                    LCD.SetFontColor(MEDIUMSEAGREEN);
                    LCD.FillRectangle(obstacleCoord[j][0], obstacleCoord[j][1], 50, 50);
                    obstacleCoord[j][0] = 225;
                    obstacleCoord[j][1] = 200;
                }
            }else{
                // Makes sure obstacles no longer in the field of play are put in the 'dead zone', not colored black, and a counter is incremented for loop control
                obstaclesCompletedCounter++;
                obstacleCoord[j][0] = 225;
                obstacleCoord[j][1] = 200;
            }
        }

        // Loops through coins and displays only the number of coins that should be displayed
        LCD.SetFontColor(GOLD);
        for(int l = 0; l < coinArrayIndex; l++){

            // Makes sure coin is still in field of play and not been hit yet
            if(coinCoord[l][0] > 30 && coinCoord[l][0] != 225){

                // Draws Coin
                LCD.FillCircle(coinCoord[l][0], coinCoord[l][1], 25);

                // Checks if the coin and kart are within a certain range to constitute a collision
                if((coinCoord[l][0] - player.getXCoord() < 75) && (abs(coinCoord[l][1] - player.getYCoord()) < 75)){

                    // 'Deletes' coin by blending it with the background and changing the coordinates to the 'dead zone'
                    LCD.SetFontColor(MEDIUMSEAGREEN);
                    LCD.FillCircle(coinCoord[l][0], coinCoord[l][1], 25);
                    coinCoord[l][0] = 225;
                    coinCoord[l][1] = 200;

                    // Plays coin sound effect
                    Buzzer.Tone(FEHBuzzer::G6, 100);
                    Buzzer.Tone(FEHBuzzer::C7, 100);

                    // Increments the coins collected counter
                    points++;

                }
            }else{
                // Makes sure coins no longer in the field of play are put in the 'dead zone' and not colored gold
                coinCoord[l][0] = 225;
                coinCoord[l][1] = 200;
            }

        }

        /*
         * Adjusts Speed
         */

        // Adjusts sleep time, refresh rate, based on current player speed
        // REQUIRED SWITCH CASE
        switch(player.getSpeed()){

        case 1:
            Sleep(1.0);
            break;
        case 2:
            Sleep(.8);
            break;
        case 3:
            Sleep(.6);
            break;
        case 4:
            Sleep(.4);
            break;
        }

        /*
         * Removes all obstacles to allow a refresh in positions
         */

        // Loops through all obstacles that have been released
        LCD.SetFontColor(MEDIUMSEAGREEN);
        for(int k = 0; k < obstacleArrayIndex; k++){

            // If that obstacle is in the field of play then it 'deletes' it from the screen
            if(obstacleCoord[k][0] > 0){
                LCD.FillRectangle(obstacleCoord[k][0], obstacleCoord[k][1], 50, 50);

                // Moves obstacle as long as it isn't in the 'dead zone'
                if(obstacleCoord[k][0] != 225){
                    obstacleCoord[k][0] -= 20;
                }
            }
        }

        // Loops through all coins that have been released
        LCD.SetFontColor(MEDIUMSEAGREEN);
        for(int m = 0; m < coinArrayIndex; m++){

            // If that coin is in the field of play then it 'deletes' it from the screen
            if(coinCoord[m][0] > 0){
                LCD.FillCircle(coinCoord[m][0], coinCoord[m][1], 25);

                // Moves coin as long as it isn't in the 'dead zone'
                if(coinCoord[m][0] != 225){
                    coinCoord[m][0] -= 20;
                }
            }
        }

        /*
         * Adjusts counters and stats for the next screen refresh
         */

        // Increments counter variables for letting obstacles and coins out
        obstacleCounter++;
        coinCounter++;

        // Checks if obstacle counter has reached limit
        if(obstacleCounter == 10){

            // Resets counter and increments the array index, allowing another obstacle to be released
            obstacleCounter = 0;
            obstacleArrayIndex++;
        }

        // Checks if coin counter has reached limit
        if(coinCounter == 5){

            // Checks to make sure all coins have not been released yet
            if(coinArrayIndex < NUM_COINS){

                // Resets counter and increments the array index, allowing another coin to be released
                coinCounter = 0;
                coinArrayIndex++;
            }
        }

        // Checks if time elapsed since last obstacle hit or speed increase has been five seconds
        if(TimeNow() - lastSavedTime >= 3.0){

            // Reinitializes start time to the current time
            lastSavedTime = TimeNow();

            // Increments the player's speed
            player.increaseSpeed();

        }

        // 'Hides' the player for the refresh
        LCD.SetFontColor(MEDIUMSEAGREEN);
        LCD.FillRectangle(player.getXCoord(), player.getYCoord(), 50, 50);

        /*
         * Checks for touch to allow side-to-side Kart movement
         */

        // Reinitializes touch coordinate variables to -1
        x = -1, y = -1;

        // Saves boolean value if a touch occured
        bool isTouched = LCD.Touch(&x, &y);

        // Adjusts player according to location of the touch based if touch occured
        if(isTouched){
            if(y < 111){
                player.adjustYCoordDown();
            }else{
                player.adjustYCoordUp();
            }
        }


        // Checks loop condition
    }while(obstaclesCompletedCounter < NUM_OBSTACLES);

    /*
     * Post Gameplay
     */

    // Declares score variable
    int score = 0;

    // Calculate final score based on timed and number of coins collected
    score = ((50.0)*(100.0/(TimeNow()-startTime)) + points) * 1000;

    return score;
}

int main()
{

    // Calls first function
    displaySplashScreen();
    displayMenu();

    return 0;
}
