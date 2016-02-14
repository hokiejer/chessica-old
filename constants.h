
//#define DEBUG_MODE
//#define PROFILING_MODE

#define NULLRESET (Reset *) 0
#define WHITEFIRST 1
#define BLACKFIRST 0
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#define YES 1
#define NO 0
#define WHITE 1
#define BLACK 0
#define COMPUTER_PLAYER 0
#define HUMAN_PLAYER 1
#define MAX_SCORE 125000000
#define MIN_SCORE -125000000
#define INCREASING 0
#define DECREASING 1
#define GREATER_THAN 1
#define LESS_THAN -1
#define EQUAL_TO 0
#define EMPTY -1
#define ONE_MB 1048576

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define UP_LEFT 0
#define UP_RIGHT 1
#define DOWN_LEFT 2
#define DOWN_RIGHT 3


/* Define the piece types */
#define NONE 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

//Thread-related constants
#define SEARCH_THREADS 8
#define IO_THREAD 9
#define ENGINE_THREAD 10
#define RUNS_SERIALLY 0
#define MAX_THREADS SEARCH_THREADS+2

/* Large prime number for hash value generation 
 *     This is the smallest prime greater than 100 million.  I didn't want
 * to go much higher than this, because signed ints can only go up to about
 * 2.1 billion, and I need some breathing room with ZERO_HASH */ 
#define LARGE_PRIME 100000007

/* This is the actual number of lists contained by the hash table */
#define HASH_TABLE_SIZE 100000

/* Define ZERO_HASH as five times the hash value - Adding this each time 
 *       we mod will eliminate the possibility of a negative hash value */
#define ZERO_HASH (5*LARGE_PRIME)
#define SMALLER_PRIME 151
#define HAVE_NOT_SEEN_MOVE 512.0

/* These are in megabytes 
     NOTE: This takes into consideration the number of resets seen in 
           Study mode
*/
#define HASH_TABLE_MAX_SIZE 150
#define HASH_TABLE_REDUCED_SIZE 135

/* This is the maximum value HashCount can reach */
#define MAX_HASHCOUNT 250

/* This is the number of files used to save resets */
#define NUMBER_OF_STUDY_FILES 2000

/* Length of time to allow per move in PRACTICE mode */
#define PRACTICE_MOVE_TIME 5

/* Maximum number of possible moves from a given board */
#define MAX_POSSIBLE_MOVES 100

/* Maximum number of moves in a game */
#define MAX_MOVES_PER_GAME 500

#define STILL_IN_OPENING 0
#define DONE_WITH_OPENING -1

#define DO_NOT_SAVE_MOVE -1

#define LOG_NAME "chess.log"

/* Number of seconds between polls to see if we're done studying */
#define TIME_BETWEEN_STUDYING_POLLS 10

#define NOTRANK1 0x00ffffffffffffff
#define RANK1 0xff00000000000000
#define RANK2 0x00ff000000000000
#define RANK3 0x0000ff0000000000
#define RANK4 0x000000ff00000000
#define RANK5 0x00000000ff000000
#define RANK6 0x0000000000ff0000
#define RANK7 0x000000000000ff00
#define RANK8 0x00000000000000ff
#define NOTRANK1 0x00ffffffffffffff
#define NOTRANK2 0xff00ffffffffffff
#define NOTRANK3 0xffff00ffffffffff
#define NOTRANK4 0xffffff00ffffffff
#define NOTRANK5 0xffffffff00ffffff
#define NOTRANK6 0xffffffffff00ffff
#define NOTRANK7 0xffffffffffff00ff
#define NOTRANK8 0xffffffffffffff00
#define FILE1 0x8080808080808080
#define FILE2 0x4040404040404040
#define FILE3 0x2020202020202020
#define FILE4 0x1010101010101010
#define FILE5 0x0808080808080808
#define FILE6 0x0404040404040404
#define FILE7 0x0202020202020202
#define FILE8 0x0101010101010101
#define NOTFILE1 0x7f7f7f7f7f7f7f7f
#define NOTFILE2 0xbfbfbfbfbfbfbfbf
#define NOTFILE3 0xdfdfdfdfdfdfdfdf
#define NOTFILE4 0xefefefefefefefef
#define NOTFILE5 0xf7f7f7f7f7f7f7f7
#define NOTFILE6 0xfbfbfbfbfbfbfbfb
#define NOTFILE7 0xfdfdfdfdfdfdfdfd
#define NOTFILE8 0xfefefefefefefefe
#define CANMOVEUP    0xffffffffffffff00
#define CANMOVEDOWN  0x00ffffffffffffff
#define CANMOVELEFT  0x7f7f7f7f7f7f7f7f
#define CANMOVERIGHT 0xfefefefefefefefe
#define UREDGE 0xfefefefefefefe00
#define ULEDGE 0x7f7f7f7f7f7f7f00
#define DREDGE 0x00fefefefefefefe
#define DLEDGE 0x007f7f7f7f7f7f7f
#define K0100 0xfefefefefefe0000
#define K0200 0xfcfcfcfcfcfcfc00
#define K0400 0x00fcfcfcfcfcfcfc
#define K0500 0x0000fefefefefefe
#define K0700 0x00007f7f7f7f7f7f
#define K0800 0x003f3f3f3f3f3f3f
#define K1000 0x3f3f3f3f3f3f3f00
#define K1100 0x7f7f7f7f7f7f0000
#define ULCORNER 0x0000000000000080
#define URCORNER 0x0000000000000001
#define LLCORNER 0x8000000000000000
#define LRCORNER 0x0100000000000000

#define WCASTLEKSAFE 0x0e00000000000000
#define WCASTLEQSAFE 0x3800000000000000
#define BCASTLEKSAFE 0x000000000000000e
#define BCASTLEQSAFE 0x0000000000000038
#define WCASTLEKEMPTY 0x0600000000000000
#define WCASTLEQEMPTY 0x7000000000000000
#define BCASTLEKEMPTY 0x0000000000000006
#define BCASTLEQEMPTY 0x0000000000000070

#define ENGINEACTION_WAIT 0
#define ENGINEACTION_THINK 1
#define ENGINEACTION_MOVE 2

// Structures
extern signed char InitialPieceType[32];
extern float CheckMateScore[2];
extern float DrawScore[2];

