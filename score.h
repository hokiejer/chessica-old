
/* score.h */

/* Score changes for various moves */
#define VERY_GOOD_MOVE 0.5
#define GOOD_MOVE 0.3
#define EVEN_MOVE 0.0
#define BAD_MOVE -0.3

#define DRAW_SCORE -3141590
#define CHECKMATE_SCORE 101000000

#define STACKED_PAWN_PENALTY 0.6
#define ISOLATED_PAWN_PENALTY 0.6
#define STACKED_AND_ISOLATED_PAWN_PENALTY 0.36
#define PAWN_PROTECTING_PAWN_BONUS 0.05
#define PAWN_ON_RANK_7_BONUS 1.0
#define PAWN_ON_RANK_6_BONUS 0.5
#define PAWN_ON_RANK_5_BONUS 0.05

/* Assume average mobility for a knight.  Anything less will incur
 * a penalty, anything more will give a bonus. */
#define AVERAGE_KNIGHT_MOBILITY 4
#define KNIGHT_MOBILITY_PENALTY 0.02
#define KNIGHT_MOBILITY_BONUS 0.02

/* Assume average mobility for a bishop.  Anything less will incur
 * a penalty, anything more will give a bonus. */
#define AVERAGE_BISHOP_MOBILITY 9
#define BISHOP_MOBILITY_PENALTY 0.02
#define BISHOP_MOBILITY_BONUS 0.02
#define BISHOP_MAIN_DIAGONAL_BONUS 0.02

/* Bonus for having more bishops of one color than your opponent */
#define UNBALANCED_BISHOPS_BONUS 0.05

/* Bonus for having rooks on open or half-open files */
#define ROOK_OPEN_FILE_BONUS 0.05
#define ROOK_HALF_OPEN_FILE_BONUS 0.02

/* Bonus for being in castled position */
#define ABILITY_TO_CASTLE_BONUS 0.02
#define CASTLED_KING_BONUS 0.05

