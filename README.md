# Widescreen patch for the game 'M: Alien Paranoia'
A simple console Windows C++ app that patches the main game executable and replaces a specific hex sequence using the dictionary available on the Widescreen Gaming Forum (WSGF).
Multiple aspect ratios are supported.

Only the aspect ratio of 3D renderer is changed. The menu and HUD elements will remain stretched.

# Download
A compiled binary file is available on the Releases page.

# Usage
1. Download and run the Widescreen patch
2. Choose one of the provided widescreen ratios: 16:9, 16:10 or 21:9 and press Enter
3. The system Open File dialog will pop up. Navigate to the game directory and select 'alien.exe'
4. After the patch is done, run Setup.exe located in the game directory and choose the appropriate resolution

# Uninstall
The Widescreen patch will create a backup of the unmodified executable with added '.bak' extension. If you want to revert the game back to its original state, simply delete the patched 'alien.exe' and rename the backed up 'alien.exe.bak' file removing the '.bak' part.

# Resolutions wider than 1920 pixels
If you intend to set a resolution wider than 1920 pixels, you will most likely need to use DirectX wrapper such as dgVoodoo, as the game might fail to start.

If you drop dgVoodoo's files into the game directory, run the setup.exe again AFTER that, so it will detect the dgVoodoo's virtual GPU and enumerate its resolutions properly.

# Notes
If the game is installed in the Program Files directory and you're running Windows Vista or newer, you will either need to run the Widescreen patch with administrative priveleges or reinstall the game to a path outside of system write-protected directories.

Dont use the in-game menu to change the resolution. It won't enumerate the widscreen resolutions at all and might crash the game when changing mode. Use the 'Setup.exe' utility located in the game directory.

Every aspect ratio was tested and the game seems to run properly with any of them.
