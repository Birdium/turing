; This example program checks if the input string 's length is a square.
; Input: a string of 1's, e.g. '1111111111111111111111111'

; the finite set of states
#Q = {0,move,add,move_back,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {1,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 1_ 11 ** move
0 __ __ ** reject

; State move: ready to remove 2k+1's 1 
move 11 _1 rr move
move 1_ 11 *r add
move __ __ ** accept
move _1 __ ** reject

; State add: add 1 to tape1
add 1_ 11 *l move_back

; State move_back: move_back tape1
move_back 11 11 *l move_back
move_back 1_ 1_ *r move

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: write 'false' on 1st tape
reject 00 __ rl reject
reject 01 __ rl reject
reject 10 __ rl reject
reject 11 __ rl reject
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject
