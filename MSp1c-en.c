/**
 * C-Implementation of the practice, to have a high-level 
 * functional version with all the features you have to implement 
 * in assembly language.
 * From this code calls are made to assembly subroutines. 
 * THIS CODE CANNOT BE MODIFIED AND SHOULD NOT BE DELIVERED. 
 */
 
#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO


/**
 * Constants
 */
#define DIMMATRIX  10    //matrix dimension
#define SIZEMATRIX DIMMATRIX*DIMMATRIX  //=100

/**
 * Global variables definition
 */
//extern int developer;	 //Declared variable in assembly language indicating the program developer name.

// Matrix 10x10 where we have the mines (There are 20 marked mines)
char mines[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ','*',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ','*',' ','*',' ',' ',' ',' '},
                                     {' ','*','*','*','*','*','*','*',' ',' '},
                                     {' ',' ',' ','*',' ','*',' ',' ',' ',' '},
                                     {' ',' ','*','*','*','*','*','*',' ',' '},
                                     {' ',' ',' ','*',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' ','*'} };

// Matrix 10x10 with the marked initial mines (1 mine marked).
char marksIni[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ','M'} };
// Matrix 10x10 with the marked mines.
char marks[DIMMATRIX][DIMMATRIX];
                                     
short numMines = 19;// Mines that remain to be marked.

int  rowScreen;	    //Row to place the cursor on the screen.
int  colScreen;	    //Column to place the cursor on the screen.
char charac;        //Character read from keyboard and to display on the screen.
char state    = '1';//State of the game.
                    // '0': Exit, we have pressed the 'ESC' key to exit.
					// '1': We keep playing.
					// '2': Won, all mines have been marked.

int  indexMat = 54; //Index to access mines and marks matrix from assembly language.			


/**
 * Definition of assembly language subroutines called from C.
 */
void countMinesP1();
void showMinesP1();
void showCharBoardP1();
void moveCursorP1();
void mineMarkerP1();
void checkEndP1();
void playP1();

/**
 * Definition of C functions.
 */
void clearscreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();

void printMenuP1_C();
void printBoardP1_C();

void countMinesP1_C();
void showMinesP1_C();
void showCharBoardP1_C();
void moveCursorP1_C();
void mineMarkerP1_C();
void checkEndP1_C();

void printMessageP1_C();
void playP1_C();


/**
 * Clear screen.
 * 
 * Global variables used:	
 * None
 * 
 * This function is not called from assembly code
 * and an equivalent assembly subroutine is not defined.
 */
void clearScreen_C(){
	
    printf("\x1B[2J");
    
}


/**
 * Place the cursor at a position on the screen.
 * 
 * Global variables used:	
 * (row) : Row of the screen where the cursor is placed.
 * (col) : Column of the screen where the cursor is placed.
 * 
 * An assembly language subroutine 'gotoxyP1' is defined to be able 
 * to call this function saving the status of the processor registers. 
 * This is done because C functions do not maintain the status of 
 * the processor registers.
 */
void gotoxyP1_C(){
	
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Show a character on the screen at the cursor position.
 * 
 * Global variables used:	
 * (charac) : Character to show.
 * 
 * An assembly language subroutine 'printchP1' is defined to be able 
 * to call this function saving the status of the processor registers. 
 * This is done because C functions do not maintain the status of 
 * the processor registers. The parameters are equivalent.
 */
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Read a character from the keyboard without displaying it 
 * on the screen and store it in the variable (charac).
 * 
 * Global variables used:	
 * (charac) : Character read from the keyboard.
 * 
 * An assembly language subroutine 'getchP1' is defined to be able 
 * to call this function saving the status of the processor registers. 
 * This is done because C functions do not maintain the status of 
 * the processor registers. The parameters are equivalent.
 */
void getchP1_C(){

   static struct termios oldt, newt;

   /*tcgetattr get terminal parameters
   STDIN_FILENO indicates that standard input parameters (STDIN) are written on oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*copy parameters*/
   newt = oldt;

   /* ~ICANON to handle keyboard input character to character, not as an entire line finished with /n
      ~ECHO so that it does not show the character read*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fix new terminal parameters for standard input (STDIN)
   TCSANOW tells tcsetattr to change the parameters immediately.*/
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Read a character*/
   charac=(char)getchar();                 
    
   /*restore the original settings*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}


/**
 * Show the game menu on the screen and ask for an option.
 * Only accepts one of the correct menu options ('0'-'9').
 * 
 * Global variables used:	
 * (rowScreen) : Row of the screen where the cursor is placed.
 * (colScreen) : Column of the screen where the cursor is placed.
 * (charac)    : Character read from the keyboard.
 * (developer) :((char *)&developer):Variable defined in the assembly code.
 * 
 * This function is not called from the assembly code and 
 * an equivalent subroutine has not been defined in assembly language.
 */
void printMenuP1_C(){
	clearScreen_C();
    rowScreen = 1;
    colScreen = 1;
    gotoxyP1_C();
    printf("                                     \n");
    printf("            Developed by:            \n");
    printf(" ___________________________________ \n");
    printf("|                                   |\n");
    printf("|          MENU MINESWEEPER         |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|          1. countMines            |\n");
    printf("|          2. showMines             |\n");
    printf("|          3. showCharBoard         |\n");
    printf("|          4. moveCursor            |\n");
    printf("|          5. mineMarker            |\n");
    printf("|          6.                       |\n");
    printf("|          7. checkEnd              |\n");
    printf("|          8. Play Game             |\n");
    printf("|          9. Play Game C           |\n");
    printf("|          0. Exit                  |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|             OPTION:               |\n");
    printf("|___________________________________|\n"); 
    
    charac=' ';
    while (charac < '0' || charac > '9') {
      rowScreen = 21;
      colScreen = 23;
      gotoxyP1_C(); 
	  getchP1_C(); 
	  printchP1_C();
	}
	
}


/**
 * Show the game board on the screen. Lines of the board.
 * 
 * Global variables used:
 * (rowScreen) : Row of the screen where the cursor is placed.
 * (colScreen) : Column of the screen where the cursor is placed.
 *  
 * This function is not called from the assembly code and 
 * an equivalent subroutine has not been defined in assembly language.
 */
void printBoardP1_C(){

   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();
   printf(" _____________________________________________ \n");	//01
   printf("|                       |                     |\n");	//02
   printf("|      MINESWEEPER      |  Mines to  Mark: __ |\n");	//03
   printf("|_______________________|_____________________|\n");	//04
   printf("|     0   1   2   3   4   5   6   7   8   9   |\n");	//05
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//06
   printf("| 0 |   |   |   |   |   |   |   |   |   |   | |\n");	//07
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//08
   printf("| 1 |   |   |   |   |   |   |   |   |   |   | |\n");	//09
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//10
   printf("| 2 |   |   |   |   |   |   |   |   |   |   | |\n");	//11
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//12
   printf("| 3 |   |   |   |   |   |   |   |   |   |   | |\n");	//13
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//14
   printf("| 4 |   |   |   |   |   |   |   |   |   |   | |\n");	//15
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//16
   printf("| 5 |   |   |   |   |   |   |   |   |   |   | |\n");	//17
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//18
   printf("| 6 |   |   |   |   |   |   |   |   |   |   | |\n");	//19
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//20
   printf("| 7 |   |   |   |   |   |   |   |   |   |   | |\n");	//21
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//22
   printf("| 8 |   |   |   |   |   |   |   |   |   |   | |\n");	//23
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//24
   printf("| 9 |   |   |   |   |   |   |   |   |   |   | |\n");	//25
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//26
   printf("|_____________________________________________|\n");	//27
   printf("|                                             |\n");	//28
   printf("|                                             |\n");	//29
   printf("|_____________________________________________|\n");	//30
   printf("|                                             |\n");	//31
   printf("|   (m)Mark Mine                  (ESC)Exit   |\n"); //32
   printf("|   (i)Up    (j)Left    (k)Down    (l)Right   |\n"); //33
   printf("|_____________________________________________|\n");	//34
   
}


/**
 * Go through the matrix (mines) to count the numbers of mines there are.
 * 
 * Global variables :	
 * (mines)    : Matrix where we have the mines.
 * (numMines) : Mines that remain to be marked.
 *  
 * This function is not called from aassembly code.
 * An equivalent assembly language subroutine 'countMinesP1' is defined.
 */
void countMinesP1_C(){
   int i,j;
   
   numMines = 0;
   for (i=0;i<DIMMATRIX;i++){
	  for (j=0;j<DIMMATRIX;j++){
         if(mines[i][j]=='*') numMines++;
      }
   }
   
}


/**
 * Converts the value (numMines), mines that remain to be marked,
 * (value between 0 and 99) in two ASCII characters.
 * The value (numMines) must be divided by 10, the quotient will represent
 * the tens (tens) and the residue the units (united), and then
 * must be converted to ASCII by adding 48, character '0'.
 * Show the digits (ASCII characters) of the tens in row 3,
 * column 44 of the screen and the units in row 3, column 45.
 * To place the cursor you must call the function gotoxyP1_C and
 * to show characters the printchP1_C function.
 * 
 * Global variables :		
 * (rowScreen) : Row to place the cursor on the screen.
 * (colScreen) : Column to place the cursor on the screen.
 * (numMines)  : Mines that remain to be marked.
 * (charac)    : Character read from keyboard and to display on the screen.
 * 
 * This function is not called from aassembly code.
 * An equivalent assembly language subroutine 'showMinesP1' is defined.
 */
 void showMinesP1_C() {
	
	short tens  = numMines/10;//Tens
	short units = numMines%10;//Units
	
	rowScreen = 3;
	colScreen = 44;
	gotoxyP1_C();   
	charac = tens + '0';
	printchP1_C();
	
	colScreen = 45;
	gotoxyP1_C();   
	charac = units + '0';
	printchP1_C();
	
}


/**
 * Show the character in the position indicated by (indexMat) of the
 * (marks) matrix at the position the cursor on the screen inside the
 * dashboard calculated based on the index of the matrix (indexMat). 
 * Leave the cursor at the position where we show the character.
 * To calculate the position of the cursor on the screen use
 * these formulas:
 * rowScreen = ((indexMat / 10) * 2) + 7
 * colScreen = ((indexMat % 10) * 4) + 7
 * To place the cursor you must call the function gotoxyP1_C and
 * to show the character the function printchP1_C.
 * 
 * Global variables :	
 * (marks)     : Matrix with the marked mines. and the number of mines in the open positions.
 * (indexMat)  : Index to access matrices from assembly language.
 * (rowScreen) : Row to place the cursor on the screen.
 * (colScreen) : Column to place the cursor on the screen.
 * (charac)    : Character read from keyboard and to display on the screen.
 * 
 * This function is not called from assembly code.
 * An equivalent assembly language subroutine 'showCharBoardP1' is defined. 
 */void showCharBoardP1_C() {
   
   int row, col;
   
   row = (indexMat/10);
   col = (indexMat%10);
   
   rowScreen=(row*2)+7;
   colScreen=(col*4)+7;
   
   gotoxyP1_C();
   
   charac = marks[row][col];
   printchP1_C();
   
   gotoxyP1_C();
   
}


/**
 * Update the position of the cursor on the board, which we have indicated
 * with the variable (indexMat), depending on the key pressed (charac).
 * If the cursor goes out the board do not update the cursor position.
 * (i: up, j: left, k: down, l: right)
 * Up and down: (indexMat = indexMat +/- 10)
 * Right and left: (indexMat = indexMat +/- 1)
 * The cursor should not be positioned on the screen. 
 *  
 * Global variables :	
 * (charac)   : Character read from keyboard and to display on the screen.
 * (indexMat) : Index to access matrices from assembly language.  
 * 
 * This function is not called from assembly code.
 * An equivalent assembly language subroutine 'moveCursorP1' is defined.
 */
void moveCursorP1_C(){
	
   int row = indexMat/10;
   int col = indexMat%10; 
   
   switch(charac){
      case 'i': //amunt
         if (row>0) indexMat=indexMat-10;
      break;
      case 'j': //esquerra
         if (col>0) indexMat--;
      break;
      case 'k': //avall
         if (row<DIMMATRIX-1) indexMat=indexMat+10;
      break;
      case 'l': //dreta
		 if (col<DIMMATRIX-1) indexMat++;
      break;     
	}
	
}


/**
 * Mark / unmark a mine in the matrix (marks) at the current position 
 * of the cursor, indicated by the variable (indexMat).
 * If in that position of the matrix (marks) there is a space and
 * we haven't marked all the mines, we mark a mine by storing an 'M' into
 * the matrix (marks) and decrease the number of mines that remain
 * to be marked (numMines), if in that position of the matrix (marks) 
 * there is an 'M', we will store an space (' ') in the matrix (marks) 
 * and increase the number of mines that remain to be marked (numMines).
 * If there is another value we will not change anything.
 * The matrix shouldn't be shown, only update the matrix (marks) 
 * and the variable (numMines).
 * 
 * Global variables :	
 * (marks)    : Matrix with the marked mines.
 * (indexMat) : Index to access matrices from assembly language.
 * (numMines) : Mines that remain to be marked.
 * 
 * This function is not called from assembly code.
 * An equivalent assembly language subroutine 'mineMarkerP1' is defined.
 */
void mineMarkerP1_C() {
	
	//// To access the matrix in assembly language it is not necessary to do this calculation.
	int row = indexMat/10;
    int col = indexMat%10;
    
	if (marks[row][col] == ' ' && numMines > 0) {
		marks[row][col] = 'M';      //Mark
		numMines--;
	} else {
		if (marks[row][col] == 'M') {
			marks[row][col] = ' ';  //Unmark
			numMines++;
		}
	}
		
} 


/**
 * Check if we have marked all the mines.
 * If (numMines = 0) change the state (stata='2') "Win the game".
 * 
 * Global variables :	
 * (numMines) : Mines that remain to be marked.
 * (state)    : State of the game.
 * 
 * This function is not called from assembly code.
 * An equivalent assembly language subroutine  'checkEndP1' is defined.
 */
void checkEndP1_C() {
	
	if (numMines == 0) {
		state = '2';
	}
	
} 


/**
 * Show a message below the dashboard according to the value of the variable
 * (status) which is received as a parameter.
 * status: '0': 'ESC' is pressed to exit the game.
 *         '1': We keep playing.
 *         '2': Win, all mines have been marked.
 * Press a key to continue.
 *  
 * Global variables :	
 * (rowScreen) : Row to place the cursor on the screen.
 * (colScreen) : Column to place the cursor on the screen.
 * (state)     : State of the game.
 * 
 * This function is called from the assembly code and 
 * an equivalent subroutine has not been defined in assembly language.
 */
void printMessageP1_C() {

   rowScreen = 29;
   colScreen = 14;
   gotoxyP1_C();  
   
   switch(state){
      case '0':
         printf("<<<< EXIT: ESC >>>>");
        break;
      case '2':
         printf("++++ YOU WIN ! ++++");
      break;
   }
   
   getchP1_C();
   
}
 

/**
 * Minesweeper game
 * Main function of the game
 * Allows you to play the minesweeper game by calling all the functionalities.
 *
 * Pseudo code:
 * Initialize game status, (state = '1').
 * Show the game board (call the PrintBoardP1_C function).
 * Copy the matrix (marksIni) to (marks) with the initial marks.
 * Show on the board the movements initially marked in (marksIni).
 * Decrease (numMines) according to the mines initially marked.
 * Initialize (indexMat = 54) to indicate the initial cursor position.
 *
 * While (state = '1') do:
 *   Show on the board the remaining mines to mark
 *     calling the showMinesP1_C function.
 *   Show the character of the position indicated by (indexMat) of the
 *     (marks) matrix at the position the cursor on the screen inside
 *     of the board based on the index of the matrix (indexMat) and
 *     leave the cursor in the position where we showed the character
 *     calling the showCharBoarP1_C function.
 *   Read a key and store it to the local variable (charac) by calling
 *     the getchP1_C function.
 *   Depending on the key read we will call the corresponding function.
 *     - ['i', 'j', 'k' or 'l']     (call the moveCursorP1_C function).
 *     - 'm'                        (call the mineMarkerP1_C function).
 *     - '<ESC>' (ASCII code 27) set (state = '0') to exit.
 *   Check if we have marked all the mines calling the checkEndP1_C function.
 * End while.
 * To exit:
 *   Show on the board the remaining mines to mark
 *     calling the showMinesP1_C function.
 *   Show the character of the position indicated by (indexMat) of the
 *     (marks) matrix at the position the cursor on the screen inside
 *     of the board based on the index of the matrix (indexMat) and
 *     leave the cursor in the position where we showed the character
 *     calling the showCharBoarP1_C function.
 *   Show the corresponding output message by calling the function
 *     printMessageP1_C.
 * The game is over.
 * 
 * Global variables :
 * (state)    : State of the game.
 * (marksIni) : Matrix with the marked initial mines.
 * (marks)    : Matrix with the marked mines. and the number of mines in the open positions.
 * (indexMat) : Index to access matrices from assembly language.
 * (numMines) : Mines that remain to be marked.
 * (charac)   : Character read from keyboard and to display on the screen.
 *  
 * This function is not called from assembly code.
 * An equivalent assembly language subroutine 'playP1' is defined.
 */
void playP1_C(){
	
   state = '1';               //State to begin the game
   
   printBoardP1_C();
   
   for(int i=0; i<DIMMATRIX; i++){      //Copy initial marks
	 for(int j=0; j<DIMMATRIX; j++){
	   marks[i][j]=marksIni[i][j];
	 }
   }
   countMinesP1_C(); 	      //Count the numbers of mines.
   indexMat = 99;
   showCharBoardP1_C();       //Mine initially marked in marks.
   numMines--;

   indexMat = 54;	     
   
   while (state == '1') {     //Main loop. 
	 showMinesP1_C();
	 showCharBoardP1_C();
     
     getchP1_C();             //Read a key.
	
     if (charac>='i' && charac<='l') {  //Move cursor
       moveCursorP1_C();
     }
     if (charac=='m') {       //Mark a mine.
       mineMarkerP1_C();
     }
     if (charac==27) {        //Exit the program.
       state = '0';
     }
     
     checkEndP1_C();
   }
   showMinesP1_C();
   showCharBoardP1_C();          
   printMessageP1_C();	 //Message to indicate how the program ends.
   
}


/**
 * Main Program
 * 
 * ATTENTION: In each option an assembly subroutine is called.
 * Below them there is a line comment with the equivalent C function 
 * that we give you done in case you want to see how it works.
 */
int main(void){   

   for(int i=0; i<DIMMATRIX; i++){      //Copy initial marks.
	 for(int j=0; j<DIMMATRIX; j++){
	   marks[i][j]=marksIni[i][j];
	 }
   }
   
   int op=-1;
   while (op!='0') {
	  clearScreen_C();
      printMenuP1_C();		  //Show the menu and reads an option.
      op = charac;
      switch(op){
        case '1':  //Count the number of mines in the mines matrix.
          printf(" %c",op);
          clearScreen_C();
          printBoardP1_C();
          //=======================================================
          countMinesP1();
          //countMinesP1_C();
          //=======================================================
          showMinesP1_C();
          rowScreen = 29;
          colScreen = 20;
          gotoxyP1_C();
          printf(" Press any key ");
          getchP1_C();
        break;
        case '2':  //Show the number of mines that remain to be marked.
          printf(" %c",op);
          clearScreen_C();  
          printBoardP1_C();   
          countMinesP1_C();
          //=======================================================
          //showMinesP1();
          showMinesP1_C(); 
          //=======================================================
          rowScreen = 29;
          colScreen = 20;
          gotoxyP1_C();
          printf(" Press any key ");
          getchP1_C();
        break;
        case '3': //Show a position of the marks matrix and place the cursor.
          printf(" %c",op);
          clearScreen_C();    
          printBoardP1_C();
          indexMat = 99;
          //=======================================================
          //showCharBoardP1();
          showCharBoardP1_C();  
          //=======================================================
          rowScreen = 29;
          colScreen = 20;
          gotoxyP1_C();
          printf("Press any key ");
          getchP1_C();
        break;
        case '4':  //Move cursor.
          clearScreen_C(); 
          printBoardP1_C();
          rowScreen = 29;
          colScreen = 3;
          gotoxyP1_C();
          printf("Move cursor: i:Up, j:Left, k:Down, l:Right");
          indexMat = 22;    
          showCharBoardP1_C();
          getchP1_C();	
	      if (charac >= 'i' && charac <= 'l') { 
		    //===================================================
		    //moveCursorP1();
		    moveCursorP1_C();
		   //===================================================
		    rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
		    printf("              Press any key               ");
		    showCharBoardP1_C();
          } else {
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("             Incorrect option             ");
		  }
          getchP1_C();
        break;
        case '5': //Mark a mine.
          clearScreen_C();  
          printBoardP1_C();
          showMinesP1_C();
          rowScreen = 29;
          colScreen = 3;
          gotoxyP1_C();
          printf("        Mark a Mine: m:mark/unmark        ");
          indexMat=33;      
          showCharBoardP1_C();
          getchP1_C();
   		  if (charac=='m') {
			//===================================================
            //mineMarkerP1();
			mineMarkerP1_C();
			//===================================================
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
		    printf("              Press any key               ");
		    showMinesP1_C();
		    showCharBoardP1_C();
		    getchP1_C();
		  } else {
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("             Incorrect option             ");
			getchP1_C();
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("                                          ");
          }
        break;
        case '6': //Count how many mines there are around the current position of the cursor.
          rowScreen = 24;
          colScreen = 3;
          gotoxyP1_C();
		  printf("        Incorrect option         ");
		  getchP1_C();
		  rowScreen = 24;
          colScreen = 3;
          gotoxyP1_C();
		  printf("                                 ");
		  
		break;
        case '7': //Check if all the mines are marked.
          clearScreen_C(); 
          printBoardP1_C();
          showMinesP1_C();
 		  //===================================================
          //checkEndP1();
		  checkEndP1_C();
		  //===================================================
		  if (state=='1') {
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("              Not Finished!!              ");
			getchP1_C();
		  } else {
			printMessageP1_C(state);
		  }
        break;
        case '8': //Complete game in assembly language,
          clearScreen_C();
          //=======================================================
          //playP1();
          //=======================================================
        break;
        case '9': //Complete game in C.
          clearScreen_C();
          //=======================================================
          playP1_C();
          //=======================================================
        break;
        case '0':
		break;	 
      }
   }
   printf("\n\n");
   
   return 0;
}
