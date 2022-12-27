; This example program right rotate the input string in the same place
; Input: a string of 0's and 1's, e.g. '11101001'

; the finite set of states
#Q = {0,right_move0,right_move1,rotate0,rotate1,halt}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

; the transition functions

; State 0: start state
0 _ _ * halt ; empty input, stop.
0 0 _ r right_move0 ; take 0
0 1 _ r right_move1 ; take 1

; State right_move0: right move and write 0 
right_move0 0 0 r right_move0
right_move0 1 0 r right_move1
right_move0 _ _ l rotate0

; State right_move1: right move and write 1 
right_move1 0 1 r right_move0
right_move1 1 1 r right_move1
right_move1 _ _ l rotate1

; State rotate0: move back to head and write 0
rotate0 * * l rotate0
rotate0 _ 0 * halt

; State rotate1: move back to head and write 1
rotate1 * * l rotate1
rotate1 _ 1 * halt
