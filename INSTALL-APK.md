# RetroArch S Pen + Cheats Auto-Load APK Installation

## APK Files Built - August 27, 2025

**Branch**: `unified-features-test`  
**Latest Commit**: Improved auto-load using RetroArch label sanitization API
**Status**: ✅ WORKING - Auto-load now handles complex ROM names with regions/compression

### Available APKs:

1. **RetroArch-SPen-CheatsAutoload-AArch64-20250827.apk** (17MB)
   - For 64-bit Android devices (ARM64/AArch64)
   - Supports most modern Android devices

2. **RetroArch-SPen-CheatsAutoload-32bit-20250827.apk** (17MB) 
   - For 32-bit Android devices (ARM32)
   - Legacy device compatibility

## Features Included:

### 🖊️ **S Pen Support** (from existing branch)
- Comprehensive S Pen hover detection
- Dual interaction modes for stylus input
- Side button support

### 🎮 **Auto-Load Cheats System** (NEW!) ✅ WORKING
- **Runtime Toggle**: Quick Menu → Cheats → "Auto-load cheats: ON/OFF"
- **Smart ROM Recognition**: Uses RetroArch's label sanitization API
- **Handles Complex Names**: Works with regions, compression, special characters
  - Example: `"pokemon - FireRed Version (USA, Europe).zip"` → matches `"pokemon - FireRed Version.cht"`
- **Case-Insensitive Matching**: `Pokemon.cht` matches `pokemon.gba`
- **No More Manual Browsing**: Eliminates need to navigate cheat database tree
- **Override System**: Manual cheat assignment for ROM hacks where auto-matching fails
- **Backwards Compatible**: Disabled by default, existing workflow unchanged

## Installation Steps:

1. **Enable Unknown Sources**: 
   - Go to Settings → Security → Enable "Unknown Sources" or "Install Unknown Apps"

2. **Choose Your APK**:
   - Most users: Use the **AArch64** version
   - Older devices: Use the **32-bit** version

3. **Install**: Transfer APK to device and tap to install

4. **Test Features**:
   - Load a ROM and go to Quick Menu → Cheats
   - Toggle "Auto-load cheats" to ON
   - Reload ROM to test automatic cheat discovery

## Testing Auto-Load Cheats:

### Quick Test Setup:
```bash
# Create test cheat file via ADB
adb shell 'echo "cheats = 2

cheat0_desc = \"Infinite Lives\"
cheat0_code = \"12345678 00FF\"
cheat0_enable = false

cheat1_desc = \"Invincibility\"
cheat1_code = \"87654321 0001\"
cheat1_enable = false" > "/storage/emulated/0/RetroArch/cheats/mgba/YourGameName.cht"'
```

### Testing Steps:
1. **Create cheat database directory**: `/storage/emulated/0/RetroArch/cheats/[core_name]/`
2. **ROM Name Matching**: Cheat files should match **sanitized** ROM names:
   - ROM: `"Super Mario Bros (USA).zip"` → Cheat: `"Super Mario Bros.cht"`
   - ROM: `"pokemon - FireRed Version (USA, Europe).zip"` → Cheat: `"pokemon - FireRed Version.cht"`
3. **Enable**: "Auto-load cheats: ON" in Quick Menu → Cheats  
4. **Load ROM**: Cheats should appear automatically in cheats menu!
5. **Debug**: Check logs for `[Cheats][auto]` messages showing sanitization process

## Override System for ROM Hacks:

If auto-matching fails (ROM hacks, different file names):
1. Go to Quick Menu → Cheats
2. Select "Set cheat override for this game" 
3. Browse and select the correct .cht file
4. Override takes priority over auto-matching for that game

## Technical Implementation:
- **Uses RetroArch APIs**: `path_get(RARCH_PATH_BASENAME)`, `label_remove_parens_and_brackets()`
- **Smart Sanitization**: Removes regions `(USA, Europe)`, handles compression `.zip`
- **Case-Insensitive**: `string_is_equal_case_insensitive()` for exact matching
- **Fallback Logic**: Auto-load → Override → Manual selection
- **Enhanced Logging**: `[Cheats][auto]` prefix for debugging

## Version Info:
- **RetroArch Version**: 1.21.0 (Updated with improved auto-load)
- **Build Date**: August 27, 2025 (Multiple iterations)
- **Build Type**: Debug (unsigned)
- **Target Android**: API 16+ (Android 4.1+)
- **Status**: ✅ **AUTO-LOAD WORKING** - Bounty feature completed!

**Ready for production testing! 🎯**