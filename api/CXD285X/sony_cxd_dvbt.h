/*------------------------------------------------------------------------------
  Copyright 2016 Sony Semiconductor Solutions Corporation

  Last Updated    : 2016/06/24
  Modification ID : 3b74e280b7ad8ce430b6a9419ac53e8f2e3737f9
------------------------------------------------------------------------------*/
/**
 @file    sony_cxd_dvbt.h

          This file provides DVB-T related type definitions.
*/
/*----------------------------------------------------------------------------*/

#ifndef sony_cxd_DVBT_H
#define sony_cxd_DVBT_H

/*------------------------------------------------------------------------------
 Enumerations
------------------------------------------------------------------------------*/
/**
 @brief DVB-T constellation.
*/
typedef enum {
    sony_cxd_DVBT_CONSTELLATION_QPSK,            /**< QPSK */
    sony_cxd_DVBT_CONSTELLATION_16QAM,           /**< 16-QAM */
    sony_cxd_DVBT_CONSTELLATION_64QAM,           /**< 64-QAM */
    sony_cxd_DVBT_CONSTELLATION_RESERVED_3       /**< Reserved. */
} sony_cxd_dvbt_constellation_t;

/**
 @brief DVB-T hierarchy.
*/
typedef enum {
    sony_cxd_DVBT_HIERARCHY_NON,                 /**< Non-hierarchical service. */
    sony_cxd_DVBT_HIERARCHY_1,                   /**< a = 1. */
    sony_cxd_DVBT_HIERARCHY_2,                   /**< a = 2. */
    sony_cxd_DVBT_HIERARCHY_4                    /**< a = 4. */
} sony_cxd_dvbt_hierarchy_t;

/**
 @brief DVB-T code rate.
*/
typedef enum {
    sony_cxd_DVBT_CODERATE_1_2,                  /**< Code Rate : 1/2 */
    sony_cxd_DVBT_CODERATE_2_3,                  /**< Code Rate : 2/3 */
    sony_cxd_DVBT_CODERATE_3_4,                  /**< Code Rate : 3/4 */
    sony_cxd_DVBT_CODERATE_5_6,                  /**< Code Rate : 5/6 */
    sony_cxd_DVBT_CODERATE_7_8,                  /**< Code Rate : 7/8 */
    sony_cxd_DVBT_CODERATE_RESERVED_5,           /**< Code Rate : Reserved */
    sony_cxd_DVBT_CODERATE_RESERVED_6,           /**< Code Rate : Reserved */
    sony_cxd_DVBT_CODERATE_RESERVED_7            /**< Code Rate : Reserved */
} sony_cxd_dvbt_coderate_t;

/**
 @brief DVB-T guard interval.
*/
typedef enum {
    sony_cxd_DVBT_GUARD_1_32,                    /**< Guard Interval : 1/32 */
    sony_cxd_DVBT_GUARD_1_16,                    /**< Guard Interval : 1/16 */
    sony_cxd_DVBT_GUARD_1_8,                     /**< Guard Interval : 1/8  */
    sony_cxd_DVBT_GUARD_1_4                      /**< Guard Interval : 1/4  */
} sony_cxd_dvbt_guard_t;

/**
 @brief DVB-T mode.
*/
typedef enum {
    sony_cxd_DVBT_MODE_2K,                       /**< 2k mode */
    sony_cxd_DVBT_MODE_8K,                       /**< 8k mode */
    sony_cxd_DVBT_MODE_RESERVED_2,               /**< Mode Reserved 2 */
    sony_cxd_DVBT_MODE_RESERVED_3                /**< Mode Reserved 3 */
} sony_cxd_dvbt_mode_t;

/**
 @brief DVB-T HP/LP profile.
*/
typedef enum {
    sony_cxd_DVBT_PROFILE_HP = 0,                /**< High Priority */
    sony_cxd_DVBT_PROFILE_LP                     /**< Low Priority */
} sony_cxd_dvbt_profile_t;

/*------------------------------------------------------------------------------
 Structs
------------------------------------------------------------------------------*/
/**
 @brief The TPS information structure.
*/
typedef struct {
    sony_cxd_dvbt_constellation_t constellation; /**< DVB-T constellation. */
    sony_cxd_dvbt_hierarchy_t hierarchy;         /**< DVB-T hierarchy. */
    sony_cxd_dvbt_coderate_t rateHP;             /**< The HP code rate. */
    sony_cxd_dvbt_coderate_t rateLP;             /**< The LP code rate. */
    sony_cxd_dvbt_guard_t guard;                 /**< The guard interval. */
    sony_cxd_dvbt_mode_t mode;                   /**< The mode. */
    uint8_t fnum;                            /**< The f number. */
    uint8_t lengthIndicator;                 /**< The Length indicator. */
    uint16_t cellID;                         /**< The Cell ID. */
    uint8_t reservedEven;                    /**< Reserved (Even). */
    uint8_t reservedOdd;                     /**< Reserved (Odd). */
} sony_cxd_dvbt_tpsinfo_t;

#endif /* sony_cxd_DVBT_H */
