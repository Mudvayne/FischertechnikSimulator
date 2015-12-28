/*
    PROGRAM DEFINES
*/
#ifdef ON_TARGET
    #define TRAVERSE_TIME_ONE_TM                        2000
    #define RUN_LONGER_THAN_THEORETICALLY_NEEDED        500
    #define EMPTY_SPACE_TO_BE_READY                     600
    #define TRAVERSE_TIME_AFTER_CENTERED_LB             70
    #define TOOL_TIME                                   3000
    #define TIMEOUT                                     7000
    #define STAGE_SIX_LB_CHECK_FULL_TIME                3000
    #define STAGE_SIX_CHECK_INTERVAL                    3000
    #define STAGE_SIX_RUN_TM_TOO_CHECK_IF_ITEM_WAS_REMOVED_TIME     2000
#else

    #define TRAVERSE_TIME_ONE_TM                        5000
    #define RUN_LONGER_THAN_THEORETICALLY_NEEDED        500
    #define EMPTY_SPACE_TO_BE_READY                     1200
    #define TRAVERSE_TIME_AFTER_CENTERED_LB             700
    #define TOOL_TIME                                   3000
    #define TIMEOUT                                     15000 //from LB to LB
    #define STAGE_SIX_LB_CHECK_FULL_TIME                3000
    #define STAGE_SIX_CHECK_INTERVAL                    3000
    #define STAGE_SIX_RUN_TM_TOO_CHECK_IF_ITEM_WAS_REMOVED_TIME     2000
#endif

/*
    SIMULATOR DEFINES
*/

#define SLEEP_TIME 10
#define PUSHER_SPEED_PER_MS 0.0002
