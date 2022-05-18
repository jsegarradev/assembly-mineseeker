section .data               
;Change Name and Surname for your data.
;developer db "_Name _Surname_",0

;Constant that is also defined in C.
DIMMATRIX    equ 10
SIZEMATRIX   equ 100

section .text            
;Variables defined in Assembly language.
;global developer     
                         
;Global variables defined in C.
extern rowScreen, colScreen, charac, indexMat
extern mines, marksIni, marks, numMines, state

;Assembly language subroutines called from C.
global countMinesP1, showMinesP1, showCharBoardP1
global moveCursorP1, mineMarkerP1, checkEndP1, playP1	  

;C functions that are called from assembly code.
extern gotoxyP1_C, getchP1_C, printchP1_C
extern printBoardP1_C, printMessageP1_C	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATTENTION: Remember that in assembly language the variables and parameters 
;; of type 'char' must be assigned to records of type
;; BYTE (1 byte): al, ah, bl, bh, cl, ch, dl, dh, sil, dil, ..., r15b
;; those of type 'short' must be assigned to records of type
;; WORD (2 bytes): ax, bx, cx, dx, si, di, ...., r15w
;; those of type 'int' must be assigned to records of type
;; DWORD (4 bytes): eax, ebx, ecx, edx, esi, edi, ...., r15d
;; those of type 'long' must be assigned to records of type
;; QWORD (8 bytes): rax, rbx, rcx, rdx, rsi, rdi, ...., r15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; a; ;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; The assembly subroutines you need to implement are:
;;   countMinesP1, showMinesP1,  showCharBoardP1, moveCursorP1
;;   mineMarkerP1, checkEndP1.  
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This subroutine is already done. YOU CANNOT MODIFY IT.
; Place the cursor at a position on the screen
; calling the gotoxyP1_C function.
; 
; Global variables used:	
; (row) : Row of the screen where the cursor is placed.
; (col) : Column of the screen where the cursor is placed.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP1:
   push rbp
   mov  rbp, rsp
   ; We save the processor's registers' state because 
   ; the C functions do not keep the registers' state.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call gotoxyP1_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This subroutine is already done. YOU CANNOT MODIFY IT.
; Show a character on the screen at the cursor position
; calling the printchP1_C fuction.
; 
; Global variables used:	
; (charac) : Character to show.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP1:
   push rbp
   mov  rbp, rsp
   ; We save the processor's registers' state because 
   ; the C functions do not keep the registers' state.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printchP1_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This subroutine is already done. YOU CANNOT MODIFY IT.
; Read a character from the keyboard without displaying it on the screen
; and store it in the variable (charac) calling the getchP1_C function.
; 
; Global variables used:	
; (charac) : Character read from the keyboard.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP1:
   push rbp
   mov  rbp, rsp
   ; We save the processor's registers' state because 
   ; the C functions do not keep the registers' state.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
 
   call getchP1_C

   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret 
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Go through the matrix (mines) to count the numbers of mines there are.
; 
; Global variables :	
; (mines)    : Matrix where we have the mines.
; (numMines) : Mines that remain to be marked.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
countMinesP1:
   push rbp
   mov  rbp, rsp
   
   mov eax, 0		; Index
   mov ebx, 0		; Counter
   
    for:
		cmp eax, SIZEMATRIX
		jl if
		jmp endFor
		if:
			cmp byte[mines+eax], '*'
			je mineFound
			jmp endIf
			mineFound:
				inc ebx
			endIf:
		inc eax
		jmp for
		endFor:
		
	mov DWORD[numMines],ebx
			
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Converts the value (numMines), mines that remain to be marked,
; (value between 0 and 99) in two ASCII characters.
; The value (numMines) must be divided by 10, the quotient will represent
; the tens (tens) and the residue the units (united), and then
; must be converted to ASCII by adding 48, character '0'.
; Show the digits (ASCII characters) of the tens in row 3,
; column 44 of the screen and the units in row 3, column 45.
; To place the cursor you must call the subroutine gotoxyP1 and
; to show characters the printchP1 subroutine.
; 
; Global variables :		
; (rowScreen) : Row to place the cursor on the screen.
; (colScreen) : Column to place the cursor on the screen.
; (numMines)  : Mines that remain to be marked.
; (charac)    : Character read from keyboard and to display on the screen.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showMinesP1:
   push rbp
   mov  rbp, rsp

   

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Show the character in the position indicated by (indexMat) of the
; (marks) matrix at the position the cursor on the screen inside the
; dashboard calculated based on the index of the matrix (indexMat). 
; Leave the cursor at the position where we show the character.
; To calculate the position of the cursor on the screen use
; these formulas:
; rowScreen = ((indexMat / 10); 2) + 7
; colScreen = ((indexMat % 10); 4) + 7
; ToTo place the cursor you must call the subroutine gotoxyP1 and
; to show characters the printchP1 subroutine.
; 
; Global variables :	
; (marks)     : Matrix with the marked mines. and the number of mines in the open positions.
; (indexMat)  : Index to access matrices from assembly language.
; (rowScreen) : Row to place the cursor on the screen.
; (colScreen) : Column to place the cursor on the screen.
; (charac)    : Character read from keyboard and to display on the screen.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showCharBoardP1:
   push rbp
   mov  rbp, rsp
	  
   
			
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		
; Update the position of the cursor on the board, which we have indicated
; with the variable (indexMat), depending on the key pressed (charac).
; If the cursor goes out the board do not update the cursor position.
; (i: up, j: left, k: down, l: right)
; Up and down: (indexMat = indexMat +/- 10)
; Right and left: (indexMat = indexMat +/- 1)
; The cursor should not be positioned on the screen. 
;  
; Global variables :	
; (charac)   : Character read from keyboard and to display on the screen.
; (indexMat) : Index to access matrices from assembly language.  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
moveCursorP1:
	push rbp
	mov  rbp, rsp

	
	
	mov rsp, rbp
	pop rbp
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
; Mark / unmark a mine in the matrix (marks) at the current position 
; of the cursor, indicated by the variable (indexMat).
; If in that position of the matrix (marks) there is a space and
; we haven't marked all the mines, we mark a mine by storing an 'M' into
; the matrix (marks) and decrease the number of mines that remain
; to be marked (numMines), if in that position of the matrix (marks) 
; there is an 'M', we will store an space (' ') in the matrix (marks) 
; and increase the number of mines that remain to be marked (numMines).
; If there is another value we will not change anything.
; The matrix shouldn't be shown, only update the matrix (marks) 
; and the variable (numMines).
; 
; Global variables :	
; (marks)    : Matrix with the marked mines.
; (indexMat) : Index to access matrices from assembly language.
; (numMines) : Mines that remain to be marked.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
mineMarkerP1:
	push rbp
	mov  rbp, rsp

	           
	
	mov rsp, rbp
	pop rbp
	ret
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Check if we have marked all the mines.
; If (numMines = 0) change the state (stata='2') "Win the game".
; 
; Global variables :	
; (numMines) : Mines that remain to be marked.
; (state)    : State of the game.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkEndP1:
	push rbp
	mov  rbp, rsp

	
	
	mov rsp, rbp
	pop rbp
	ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Minesweeper game
; Main subroutine of the game
; Allows you to play the minesweeper game by calling all the functionalities.
;
; Pseudo code:
; Initialize game status, (state = '1').
; Show the game board (call the PrintBoardP1_C function).
; Copy the matrix (marksIni) to (marks) with the initial marks.
; Show on the board the movements initially marked in (marksIni).
; Decrease (numMines) according to the mines initially marked.
; Initialize (indexMat = 54) to indicate the initial cursor position.
;
; While (state = '1') do:
;   Show on the board the remaining mines to mark
;     calling the showMinesP1 subroutine.
;   Show the character of the position indicated by (indexMat) of the
;     (marks) matrix at the position the cursor on the screen inside
;     of the board based on the index of the matrix (indexMat) and
;     leave the cursor in the position where we showed the character
;     calling the showCharBoarP1 subroutine.
;   Read a key and store it to the local variable (charac) by calling
;     the getchP1 subroutine.
;   Depending on the key read we will call the corresponding subroutine.
;     - ['i', 'j', 'k' or 'l']     (call the moveCursorP1 subroutine).
;     - 'm'                        (call the mineMarkerP1 subroutine).
;     - '<ESC>' (ASCII code 27) set (state = '0') to exit.
;   Check if we have marked all the mines calling the checkEndP1 subroutine.
; End while.
; To exit:
;   Show on the board the remaining mines to mark
;     calling the showMinesP1 subroutine.
;   Show the character of the position indicated by (indexMat) of the
;     (marks) matrix at the position the cursor on the screen inside
;     of the board based on the index of the matrix (indexMat) and
;     leave the cursor in the position where we showed the character
;     calling the showCharBoarP1 subroutine.
;   Show the corresponding output message by calling the function
;     printMessageP1_C.
; The game is over.
; 
; Global variables :
; (state)    : State of the game.
; (marksIni) : Matrix with the marked initial mines.
; (marks)    : Matrix with the marked mines. and the number of mines in the open positions.
; (indexMat) : Index to access matrices from assembly language.
; (numMines) : Mines that remain to be marked.
; (charac)   : Character read from keyboard and to display on the screen.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
playP1:
   push rbp
   mov  rbp, rsp
   
   push rax
   push rbx

   mov BYTE[state], '1'    ;state = '1';
	
   call printBoardP1_C   ;printBoardP1_C();

   
   mov rbx, 0
   p_for1:               ;for(int i=0; i<DIMMATRIX; i++){ 
	                     ; for(int j=0; j<DIMMATRIX; j++){
     mov al, BYTE[marksIni+rbx]
     mov BYTE[marks+rbx], al;marks[i][j]=marksIni[i][j];
     inc rbx
   cmp rbx, SIZEMATRIX   ;}
   jl  p_for1            ;}
   
   call countMinesP1     ;numMines = countMinesP1_C();
   
   mov DWORD[indexMat],99;indexMat = 99;
   call showCharBoardP1  ;showCharBoardP1_C();
   dec WORD[numMines]    ;numMines--;

   mov DWORD[indexMat], 54 ;indexMat= 54;         

   p_while:              
	 cmp  BYTE[state], '1' ;while (state == '1')
     jne  p_printMessage

     call showMinesP1      ;showMinesP1_C();
     call showCharBoardP1  ;showCharBoardP1_C();
		
     call getchP1          ;getchP1_C(); 
     mov  al, BYTE[charac] 

     p_move:
     cmp al, 'i'           ;if (charac>='i' && charac<='l')
     jl  p_mark
     cmp al, 'l'           
     jg  p_mark
       call moveCursorP1   ;moveCursorP1_C();
     p_mark:
     cmp al, 'm'           ;if (charac=='m')
     jne  p_esc
       call mineMarkerP1   ;mineMarkerP1_C()
     p_esc:
     cmp al, 27            ;if (charac==27)
     jne  p_check
       mov BYTE[state], '0';state = '0';

     p_check:
     call checkEndP1     ;checkEndP1_C();

     jmp  p_while

   p_printMessage:
   call showMinesP1      ;showMinesP1_C();
   call showCharBoardP1  ;showCharBoardP1_C();          
   call printMessageP1_C ;printMessageP1_C();
    
   p_end:
   
   pop rbx
   pop rax
   		
   mov rsp, rbp
   pop rbp
   ret

