/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CHEAT_MANAGER_H
#define __CHEAT_MANAGER_H

#include <boolean.h>
#include <retro_common_api.h>

#include "../setting_list.h"

RETRO_BEGIN_DECLS

enum cheat_handler_type
{
   CHEAT_HANDLER_TYPE_EMU = 0,
   CHEAT_HANDLER_TYPE_RETRO,
   CHEAT_HANDLER_TYPE_END
};

enum cheat_type
{
   CHEAT_TYPE_DISABLED = 0,
   CHEAT_TYPE_SET_TO_VALUE,
   CHEAT_TYPE_INCREASE_VALUE,
   CHEAT_TYPE_DECREASE_VALUE,
   CHEAT_TYPE_RUN_NEXT_IF_EQ,
   CHEAT_TYPE_RUN_NEXT_IF_NEQ,
   CHEAT_TYPE_RUN_NEXT_IF_LT,
   CHEAT_TYPE_RUN_NEXT_IF_GT
};

enum cheat_search_type
{
   CHEAT_SEARCH_TYPE_EXACT = 0,
   CHEAT_SEARCH_TYPE_LT,
   CHEAT_SEARCH_TYPE_LTE,
   CHEAT_SEARCH_TYPE_GT,
   CHEAT_SEARCH_TYPE_GTE,
   CHEAT_SEARCH_TYPE_EQ,
   CHEAT_SEARCH_TYPE_NEQ,
   CHEAT_SEARCH_TYPE_EQPLUS,
   CHEAT_SEARCH_TYPE_EQMINUS
};

enum cheat_match_action_type
{
   CHEAT_MATCH_ACTION_TYPE_VIEW = 0,
   CHEAT_MATCH_ACTION_TYPE_DELETE,
   CHEAT_MATCH_ACTION_TYPE_COPY,
   CHEAT_MATCH_ACTION_TYPE_BROWSE
};

enum cheat_rumble_type
{
   RUMBLE_TYPE_DISABLED = 0,
   RUMBLE_TYPE_CHANGES,
   RUMBLE_TYPE_DOES_NOT_CHANGE,
   RUMBLE_TYPE_INCREASE,
   RUMBLE_TYPE_DECREASE,
   RUMBLE_TYPE_EQ_VALUE,
   RUMBLE_TYPE_NEQ_VALUE,
   RUMBLE_TYPE_LT_VALUE,
   RUMBLE_TYPE_GT_VALUE,
   RUMBLE_TYPE_INCREASE_BY_VALUE,
   RUMBLE_TYPE_DECREASE_BY_VALUE,
   RUMBLE_TYPE_END_LIST
};

/* Auto-load cheat result codes - for enhanced error reporting */
enum cheat_auto_load_result
{
   CHEAT_AUTO_LOAD_SUCCESS = 0,                /* Successfully loaded cheats */
   CHEAT_AUTO_LOAD_NO_CONTENT = 1,             /* No content loaded */
   CHEAT_AUTO_LOAD_NO_CORE = 2,                /* No core info available */
   CHEAT_AUTO_LOAD_NO_DATABASE = 3,            /* Cheat database path not configured */
   CHEAT_AUTO_LOAD_NO_CORE_DIR = 4,            /* Core-specific cheat directory missing */
   CHEAT_AUTO_LOAD_NO_MATCHES = 5,             /* No matching cheat files found */
   CHEAT_AUTO_LOAD_MULTIPLE_MATCHES = 6,       /* Multiple candidates found, manual selection needed */
   CHEAT_AUTO_LOAD_LOAD_FAILED = 7,            /* Cheat file found but failed to load */
   CHEAT_AUTO_LOAD_OVERRIDE_INVALID = 8        /* Override file set but invalid */
};

/* Some codes are ridiculously large - over 10000 bytes */
#define CHEAT_CODE_SCRATCH_SIZE 16*1024
#define CHEAT_DESC_SCRATCH_SIZE 255

struct item_cheat
{
   /* Clock value for when rumbling should stop */
   retro_time_t rumble_primary_end_time;
   retro_time_t rumble_secondary_end_time;

   char *desc;
   char *code;

   unsigned int idx;
   unsigned int handler;
   /* Number of bits = 2^memory_search_size
    * 0=1, 1=2, 2=4, 3=8, 4=16, 5=32
    */
   unsigned int memory_search_size;
   unsigned int cheat_type;
   unsigned int value;
   unsigned int address;
   /*
    * address_mask used when memory_search_size <8 bits
    * if memory_search_size=0, then the number of bits is 1 and this value can be one of the following:
    * 0 : 00000001
    * 1 : 00000010
    * 2 : 00000100
    * 3 : 00001000
    * 4 : 00010000
    * 5 : 00100000
    * 6 : 01000000
    * 7 : 10000000
    * if memory_search_size=1, then the number of bits is 2 and this value can be one of the following:
    * 0 : 00000011
    * 1 : 00001100
    * 2 : 00110000
    * 3 : 11000000
    * if memory_search_size=2, then the number of bits is 4 and this value can be one of the following:
    * 0 : 00001111
    * 1 : 11110000
    */
   unsigned int address_mask;
   unsigned int rumble_type;
   unsigned int rumble_value;
   unsigned int rumble_prev_value;
   unsigned int rumble_initialized;
   /* 0-15 for specific port, anything else means "all ports" */
   unsigned int rumble_port;
   unsigned int rumble_primary_strength; /* 0-65535 */
   unsigned int rumble_primary_duration; /* in milliseconds */
   unsigned int rumble_secondary_strength; /* 0-65535 */
   unsigned int rumble_secondary_duration; /* in milliseconds */

   /*
    * The repeat_ variables allow for a single cheat code to affect multiple memory addresses.
    * repeat_count - the number of times the cheat code should be applied
    * repeat_add_to_value - every iteration of repeat_count will have this amount added to item_cheat.value
    * repeat_add_to_address - every iteration of repeat_count will have this amount added to item_cheat.address
    *
    * Note that repeat_add_to_address represents the number of "memory_search_size" blocks to add to
    * item_cheat.address.  If memory_search_size is 16-bits and repeat_add_to_address is 2, then item_cheat.address
    * will be increased by 4 bytes 2*(16-bits) for every iteration.
    *
    * This is a cheating structure used for codes like unlocking all levels, giving yourself 1 of every item,etc.
    */
   unsigned int repeat_count;
   unsigned int repeat_add_to_value;
   unsigned int repeat_add_to_address;

   bool state;
   /* Whether to apply the cheat based on big-endian console memory or not */
   bool big_endian;
};

struct cheat_manager
{
   struct item_cheat working_cheat; /* retro_time_t alignment */
   struct item_cheat *cheats;
   uint8_t *curr_memory_buf;
   uint8_t *prev_memory_buf;
   uint8_t *matches;
   uint8_t **memory_buf_list;
   unsigned *memory_size_list;
   unsigned int delete_state;
   unsigned int loading_cheat_size;
   unsigned int loading_cheat_offset;
   unsigned ptr;
   unsigned size;
   unsigned buf_size;
   unsigned total_memory_size;
   unsigned num_memory_buffers;
   unsigned match_idx;
   unsigned match_action;
   unsigned search_bit_size;
   unsigned dummy;
   unsigned search_exact_value;
   unsigned search_eqplus_value;
   unsigned search_eqminus_value;
   unsigned num_matches;
   unsigned browse_address;
   char working_desc[CHEAT_DESC_SCRATCH_SIZE];
   char working_code[CHEAT_CODE_SCRATCH_SIZE];
   char current_game_override_path[PATH_MAX_LENGTH];
   bool  big_endian;
   bool  memory_initialized;
   bool  memory_search_initialized;
   bool  auto_load_enabled;
};

typedef struct cheat_manager cheat_manager_t;

extern cheat_manager_t cheat_manager_state;

unsigned cheat_manager_get_size(void);

bool cheat_manager_load(const char *path, bool append);

/**
 * cheat_manager_save:
 * @path                      : Path to cheats file (absolute path).
 *
 * Saves cheats to file on disk.
 *
 * Returns: true (1) if successful, otherwise false (0).
 **/
bool cheat_manager_save(const char *path,
      const char *cheat_database, bool overwrite);

bool cheat_manager_realloc(unsigned new_size, unsigned default_handler);

void cheat_manager_set_code(unsigned index, const char *str);

void cheat_manager_index_next(void);

void cheat_manager_index_prev(void);

void cheat_manager_toggle(bool notification_show_cheats_applied);

void cheat_manager_apply_cheats(bool notification_show_cheats_applied);

void cheat_manager_update(cheat_manager_t *handle, unsigned handle_idx);

void cheat_manager_toggle_index(bool apply_cheats_after_toggle,
      bool notification_show_cheats_applied,
      unsigned i);

unsigned cheat_manager_get_buf_size(void);

const char *cheat_manager_get_desc(unsigned i);

const char *cheat_manager_get_code(unsigned i);

bool cheat_manager_get_code_state(unsigned i);

void cheat_manager_state_free(void);

void cheat_manager_alloc_if_empty(void);

/**
 * @brief Automatically resolves and loads cheat files for currently loaded content
 *
 * Attempts to automatically find and load appropriate cheat files for the currently
 * loaded game content. Uses RetroArch's content path and core information to locate
 * matching cheat files in the configured cheat database directory.
 *
 * The function follows this priority order:
 * 1. Manual override (if set via cheat_manager_set_current_game_override)  
 * 2. Exact game name match (case-insensitive)
 * 3. Single partial match containing game name
 * 4. Multiple candidates (requires manual selection)
 *
 * Game name sanitization is applied using RetroArch's label_remove_parens_and_brackets()
 * to handle regions, compression formats, and special characters consistently.
 *
 * @param[out] loaded_exact_match Set to true if an exact name match was loaded
 * @param[out] has_multiple_candidates Set to true if multiple candidates were found
 *
 * @return Number of candidate cheat files found (0 if none, >1 if multiple)
 *
 * @note This function maintains backward compatibility and will not break existing workflows
 * @note Respects the cheats_enable_auto_load configuration setting
 * 
 * @see cheat_manager_auto_resolve_and_load_enhanced() for detailed error reporting
 * @see cheat_manager_set_current_game_override() for manual override management
 */
unsigned cheat_manager_auto_resolve_and_load_for_current_content(
    bool *loaded_exact_match, bool *has_multiple_candidates);

/**
 * @brief Enhanced cheat auto-resolution with comprehensive error reporting
 *
 * Provides the same functionality as cheat_manager_auto_resolve_and_load_for_current_content()
 * but with detailed error codes and enhanced logging for debugging and user feedback.
 * Maintains full backward compatibility while providing production-ready error handling.
 *
 * Error conditions are clearly distinguished:
 * - CHEAT_AUTO_LOAD_SUCCESS: Cheats successfully loaded
 * - CHEAT_AUTO_LOAD_NO_CONTENT: No content currently loaded
 * - CHEAT_AUTO_LOAD_NO_CORE: Core information unavailable
 * - CHEAT_AUTO_LOAD_NO_DATABASE: Cheat database path not configured
 * - CHEAT_AUTO_LOAD_NO_CORE_DIR: Core-specific cheat directory missing
 * - CHEAT_AUTO_LOAD_NO_MATCHES: No matching cheat files found
 * - CHEAT_AUTO_LOAD_MULTIPLE_MATCHES: Multiple candidates require manual selection
 * - CHEAT_AUTO_LOAD_LOAD_FAILED: Cheat file found but failed to load
 * - CHEAT_AUTO_LOAD_OVERRIDE_INVALID: Override file set but invalid/missing
 *
 * @param[out] loaded_exact_match Set to true if exact name match was loaded
 * @param[out] has_multiple_candidates Set to true if multiple candidates found  
 * @param[out] num_found Total number of matching cheat files found
 *
 * @return Detailed error code indicating success or specific failure reason
 *
 * @note Input parameters must not be NULL - function validates parameters
 * @note Automatically clears invalid overrides to maintain system consistency
 * @note Uses comprehensive logging for debugging and troubleshooting
 *
 * @see enum cheat_auto_load_result for complete error code documentation
 */
enum cheat_auto_load_result cheat_manager_auto_resolve_and_load_enhanced(
    bool *loaded_exact_match, bool *has_multiple_candidates, unsigned *num_found);

/**
 * @brief Sets a manual cheat file override for the current game
 *
 * Allows users to manually specify which cheat file should be used for the currently
 * loaded content, bypassing automatic resolution. This is particularly useful for
 * ROM hacks or games where automatic matching fails due to non-standard naming.
 *
 * The override takes priority over automatic matching and persists until:
 * - Different content is loaded
 * - Override is manually cleared via cheat_manager_clear_current_game_override()
 * - Override file becomes invalid/missing (automatically cleared)
 *
 * @param[in] cheat_file_path Full path to cheat file to use as override
 *                           Pass NULL or empty string to clear override
 *
 * @note Override is stored in memory only, not persisted to configuration
 * @note File existence is not validated at set time, only during loading
 * @note Function is safe to call with NULL or invalid paths
 *
 * @see cheat_manager_get_current_game_override() to query current override
 * @see cheat_manager_clear_current_game_override() to remove override  
 */
void cheat_manager_set_current_game_override(const char *cheat_file_path);

/**
 * @brief Retrieves the current cheat file override path
 *
 * Returns the cheat file path currently set as override for the loaded content,
 * or NULL if no override is active. The returned path should not be modified
 * and may become invalid after subsequent cheat manager operations.
 *
 * @return Current override cheat file path, or NULL if no override set
 *
 * @note Returned pointer is valid until next cheat_manager operation
 * @note Does not validate that the file still exists on disk
 * @note Returns NULL for empty override paths
 *
 * @see cheat_manager_set_current_game_override() to set override
 */
const char *cheat_manager_get_current_game_override(void);

/**
 * @brief Clears the current cheat file override
 *
 * Removes any manual override set for the current game, allowing automatic
 * cheat resolution to resume normal operation. This is equivalent to calling
 * cheat_manager_set_current_game_override(NULL).
 *
 * @note Safe to call even when no override is set
 * @note Operation is logged for debugging purposes
 *
 * @see cheat_manager_set_current_game_override() for setting overrides
 */
void cheat_manager_clear_current_game_override(void);

bool cheat_manager_copy_idx_to_working(unsigned idx);

bool cheat_manager_copy_working_to_idx(unsigned idx);

void cheat_manager_load_game_specific_cheats(const char *path_cheat_database);

void cheat_manager_save_game_specific_cheats(const char *path_cheat_database);

int cheat_manager_initialize_memory(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_exact(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_lt(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_gt(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_lte(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_gte(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_eq(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_neq(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_eqplus(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_search_eqminus(rarch_setting_t *setting, size_t idx, bool wraparound);

unsigned cheat_manager_get_state_search_size(unsigned search_size);

int cheat_manager_add_matches(const char *path,
      const char *label, unsigned type, size_t idx, size_t entry_idx);

void cheat_manager_apply_retro_cheats(void);

void cheat_manager_match_action(
      enum cheat_match_action_type match_action,
      unsigned int target_match_idx,
      unsigned int *address, unsigned int *address_mask,
      unsigned int *prev_value, unsigned int *curr_value);

int cheat_manager_copy_match(rarch_setting_t *setting, size_t idx, bool wraparound);

int cheat_manager_delete_match(rarch_setting_t *setting, size_t idx, bool wraparound);

RETRO_END_DECLS

#endif
