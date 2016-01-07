/*
    PROGRAM DEFINES
*/
#ifdef ON_TARGET
    #define STAGE_ONE_COOLDOWN_AFTER_SECOND_LB          2500
    #define PERCENTAGE_OF_TM_AT_LB1             				0.8
    //#define TRAVERSE_TIME_ONE_TM                        2000
    //#define RUN_LONGER_THAN_THEORETICALLY_NEEDED        300
    #define EMPTY_SPACE_TO_BE_READY                     800
    #define TRAVERSE_TIME_AFTER_CENTERED_LB             70
    #define DRILL_TIME                                  3000
    #define MILL_TIME                                   2000
    #define TIMEOUT                                     7000
    #define STAGE_SIX_LB_BLOCK_FOR_FULL                	1000
    #define STAGE_SIX_CHECK_INTERVAL                    3000
    #define STAGE_SIX_RUN_TM_FOR_CHECK_TIME     				2000
		#define TM1_RUNTIME																	3000
		#define TM2_RUNTIME																	2900
		#define TM3_RUNTIME																	2700
#else
    #define STAGE_ONE_COOLDOWN_AFTER_SECOND_LB          3000
    #define PERCENTAGE_OF_TM_AT_LB1             				0.7
    #define TRAVERSE_TIME_ONE_TM                        5000
    #define RUN_LONGER_THAN_THEORETICALLY_NEEDED        500
    #define EMPTY_SPACE_TO_BE_READY                     2400
    #define TRAVERSE_TIME_AFTER_CENTERED_LB             600
    #define DRILL_TIME                                  3000
    #define MILL_TIME                                   2000
    #define TIMEOUT                                     15000 //from LB to LB
    #define STAGE_SIX_LB_BLOCK_FOR_FULL                 2000
    #define STAGE_SIX_CHECK_INTERVAL                    5000
    #define STAGE_SIX_RUN_TM_FOR_CHECK_TIME             2000
		#define TM2_RUNTIME																	5300
		#define TM3_RUNTIME																	5300
#endif

/*
    SIMULATOR DEFINES
*/

#define SLEEP_TIME 10
#define PUSHER_SPEED_PER_MS 0.0002
