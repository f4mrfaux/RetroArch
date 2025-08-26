# Auto-Load Cheats Feature - Test Plan

This document outlines the test plan for the new **Auto-load cheats** feature in RetroArch.

## Feature Overview
When content loads, automatically surface the correct cheat set for that ROM in Quick Menu → Cheats, without manual browsing. Only cheats that correspond to that ROM should appear.

## Build Instructions
```bash
# From repo root  
make clean
make -j$(nproc)
./retroarch
```

## Test Prerequisites
1. **Cheat database setup**: Ensure you have cheat files in your cheat database directory structure:
   ```
   cheat_database/
   └── Core_Name/
       ├── Game_Name.cht
       ├── Game_Name_USA.cht
       ├── Game_Name_Europe.cht
       └── Other_Game.cht
   ```

2. **Sample cheat file**: Create a simple test cheat file with basic content:
   ```
   cheats = 1
   cheat0_desc = "Test Cheat"
   cheat0_code = "12345678 00000001"
   cheat0_enable = false
   ```

## Manual Test Cases

### ✅ **Test 1: Default Behavior (Backwards Compatibility)**
**Objective**: Ensure existing behavior is unchanged when auto-load is disabled (default).

**Steps**:
1. Start RetroArch
2. Load any ROM content  
3. Navigate to **Quick Menu > Cheats**
4. Verify **Auto-load cheats: OFF** is shown
5. Verify existing "Load Cheat File" and manual cheat operations work unchanged

**Expected Result**: ✅ No behavior changes, manual flow intact

---

### ✅ **Test 2: Enable Auto-Load Toggle**  
**Objective**: Verify the toggle works and persists during runtime.

**Steps**:
1. Load ROM content
2. Navigate to **Quick Menu > Cheats**
3. Select **Auto-load cheats: OFF** item
4. Verify it changes to **Auto-load cheats: ON**
5. Exit and re-enter Cheats menu
6. Verify toggle state is preserved

**Expected Result**: ✅ Toggle works, menu refreshes, state persists during session

---

### ✅ **Test 3: Exact Match Auto-Loading**
**Objective**: Test automatic loading when single exact match exists.

**Setup**:
- Place `Pokemon_Emerald.cht` in `cheat_database/mgba/`
- Ensure it's the only file matching "Pokemon_Emerald"

**Steps**:
1. Enable **Auto-load cheats: ON**  
2. Load Pokemon Emerald ROM (or equivalent test ROM)
3. Check logs for `[Cheats][auto] Exact match found: ...`
4. Navigate to **Quick Menu > Cheats**
5. Verify cheats are already loaded and toggleable

**Expected Result**: ✅ Cheats auto-loaded, immediately available in menu

---

### ✅ **Test 4: No Match Fallback**
**Objective**: Verify graceful fallback when no cheat files match.

**Setup**: 
- Use ROM with no corresponding cheat file
- Or temporarily rename cheat files to not match

**Steps**:
1. Enable **Auto-load cheats: ON**
2. Load ROM with no matching cheats  
3. Check logs - should see no auto-resolution messages
4. Navigate to **Quick Menu > Cheats**
5. Verify normal manual "Load Cheat File" options available

**Expected Result**: ✅ Falls back to manual flow, no errors or crashes

---

### ✅ **Test 5: Multiple Candidates** *(Future Enhancement)*
**Objective**: Test behavior when multiple cheat files match.

**Setup**:
- Place multiple files: `Game_USA.cht`, `Game_Europe.cht`, `Game_Japan.cht`

**Steps**:
1. Enable **Auto-load cheats: ON**
2. Load matching ROM
3. Check logs for `[Cheats][auto] Multiple candidates found: N`
4. Navigate to **Quick Menu > Cheats**

**Expected Result**: ✅ Logs multiple candidates, current implementation falls back to manual selection

---

### ✅ **Test 6: Cross-Core Compatibility** 
**Objective**: Ensure feature works across different cores/systems.

**Steps**:
1. Test with different emulator cores (e.g., mgba, snes9x, nestopia)
2. Verify cheat directory structure: `cheat_database/[core_name]/[game].cht`
3. Test auto-loading with each core type

**Expected Result**: ✅ Works consistently across all cores

---

### ✅ **Test 7: Session Reset**
**Objective**: Verify toggle resets between RetroArch sessions.

**Steps**:
1. Enable **Auto-load cheats: ON**  
2. Close RetroArch completely
3. Restart RetroArch  
4. Load ROM and check **Quick Menu > Cheats**
5. Verify toggle shows **Auto-load cheats: OFF** (default)

**Expected Result**: ✅ Toggle resets to OFF, no persistent config changes

---

### ✅ **Test 8: Error Handling**
**Objective**: Test graceful handling of edge cases.

**Test scenarios**:
- Missing cheat database directory
- Corrupted .cht files  
- Permission issues reading cheat files
- Very long file names
- Special characters in ROM/cheat names

**Expected Result**: ✅ No crashes, graceful degradation to manual mode

---

### ✅ **Test 9: Cheat File Override (ROM Hacks)**
**Objective**: Test manual override system for ROM hacks where auto-matching fails.

**Setup**:
- ROM hack file: `Pokemon_Emerald_Randomizer.gba` 
- Original cheat: `Pokemon_Emerald.cht` in `cheat_database/mgba/`
- Different cheat: `Pokemon_Ruby.cht` in `cheat_database/mgba/`

**Steps**:
1. Enable **Auto-load cheats: ON**
2. Load ROM hack (auto-matching should fail/partial match)
3. Navigate to **Quick Menu > Cheats** 
4. Select **"Set cheat override for this game"**
5. Browse to and select `Pokemon_Emerald.cht` (or any other .cht)
6. Verify menu shows **"Override: Pokemon_Emerald.cht"**
7. Exit and reload the same ROM hack
8. Verify override cheat file loads automatically
9. Select **"Clear cheat override"** 
10. Verify menu returns to **"Set cheat override for this game"**

**Expected Result**: ✅ Manual override works, takes priority over auto-matching, clears properly

---

### ✅ **Test 10: Override Cross-System Assignment**
**Objective**: Test assigning cheat files from different systems.

**Steps**:
1. Load Game Boy ROM
2. Set override to SNES cheat file from different system folder
3. Verify it loads and works (or logs appropriate errors)
4. Test with various cross-system combinations

**Expected Result**: ✅ System allows cross-assignment, handles gracefully

---

## Automated Testing Integration

### Log Messages to Verify
```bash
# Enable auto-load
[Cheats][auto] Auto-load cheats enabled

# Successful auto-resolution  
[Cheats][auto] Exact match found: /path/to/cheat.cht
[Cheats][auto] Auto-resolved and loaded cheats successfully

# Multiple candidates
[Cheats][auto] Multiple candidates found: 3
[Cheats][auto] Multiple cheat files found - check Quick Menu > Cheats

# Override operations
[Cheats][override] Set cheat override for current game: "/path/to/override.cht"
[Cheats][override] Using override cheat file: "/path/to/override.cht" 
[Cheats][override] Loaded override cheat file: "/path/to/override.cht"
[Cheats][override] Failed to load override cheat file: "/invalid/path.cht"
[Cheats][override] Cleared cheat override for current game

# Disable auto-load
[Cheats][auto] Auto-load cheats disabled
```

## Regression Testing
- [ ] Existing cheat functionality unaffected when auto-load OFF
- [ ] "Load Cheat File" still works  
- [ ] "Save Cheat File" still works
- [ ] Game-specific cheat saving/loading still works  
- [ ] Cheat search functionality still works
- [ ] Manual cheat creation still works

## Performance Considerations
- Auto-resolution only runs when:
  - Auto-load is enabled (OFF by default)
  - Content is successfully loaded  
  - Cheat database directory exists
- Minimal performance impact: single directory scan per content load

## Known Limitations  
1. **Multi-candidate selection**: Currently logs multiple matches but doesn't provide UI selection (future enhancement)
2. **No hash matching**: Uses filename matching only (database hash matching could be added later)  
3. **Runtime-only**: Settings don't persist between sessions (by design for backwards compatibility)
4. **Override scope**: Overrides are per-session, not saved permanently (prevents config file changes)

## Success Criteria
- ✅ Builds without errors
- ✅ All manual tests pass  
- ✅ Zero regressions in existing cheat functionality
- ✅ Toggle works as expected
- ✅ Auto-loading works for exact matches
- ✅ Graceful fallback for no matches
- ✅ Override system works for ROM hacks
- ✅ Cross-system override assignment possible
- ✅ Override UI updates dynamically 
- ✅ No crashes or memory leaks
- ✅ Backwards compatibility maintained