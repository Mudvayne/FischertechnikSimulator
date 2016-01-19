/*
    PROGRAM DEFINES
*/
#ifdef ON_TARGET
    #define TM1_RUNTIME									3200
    #define TM2_RUNTIME									2900
    #define TM3_RUNTIME									2700
    #define STAGE_ONE_COOLDOWN_AFTER_SECOND_LB          2500
    #define TRAVERSE_TIME_AFTER_CENTERED_LB             70
    #define DRILL_TIME                                  3000
    #define MILL_TIME                                   2000
    #define TIMEOUT                                     5000
    #define STAGE_SIX_LB_BLOCK_FOR_FULL                	2000
    #define STAGE_SIX_CHECK_INTERVAL                    2000
    #define STAGE_SIX_RUN_TM_FOR_CHECK_TIME     		1000
    #define LB2_POS                                     0.75
    #define TOOL_LB_POS                                 0.65
    #define TOOL_STAGE_WAIT_POS                         0.70
#else
    #define TM1_RUNTIME                                 5500
    #define TM2_RUNTIME									5500
    #define TM3_RUNTIME									6500
    #define STAGE_ONE_COOLDOWN_AFTER_SECOND_LB          3000
    #define TRAVERSE_TIME_AFTER_CENTERED_LB             600
    #define DRILL_TIME                                  3000
    #define MILL_TIME                                   2000
    #define TIMEOUT                                     6000
    #define STAGE_SIX_LB_BLOCK_FOR_FULL                 1500
    #define STAGE_SIX_CHECK_INTERVAL                    5000
    #define STAGE_SIX_RUN_TM_FOR_CHECK_TIME             2000
    #define LB2_POS                                     0.67
    #define TOOL_LB_POS                                 0.5
    #define TOOL_STAGE_WAIT_POS                         0.75
#endif

/*
    SIMULATOR DEFINES (C SIM)
*/

#define SLEEP_TIME 10
#define PUSHER_SPEED_PER_MS 0.0002
