# Quick Start Guide for c4recover

## Configuring c4recover for your computer

c4recover needs to be configured for your computer before it will work. To configure c4recover, open
config.xml in a text editor. Then scroll down to the "\<config\>" block, and make changes to
the following settings:

### BTS_INSTALL_DIR

Set BTS_INSTALL_DIR to the value where you installed Beyond the Sword. Some example values are as
follows:

If you installed Beyond the Sword from CD:

* "C:\Program Files (x86)\Firaxis Games\Sid Meier's Civilization 4\Beyond the Sword"

If you installed Beyond the Sword from Good Old Games (GOG):

* "C:\Program Files (x86)\GOG Galaxy\Games\Civilization IV Complete\Civ4\Beyond the Sword"

If you installed Beyond the Sword from Steam:

* "C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization IV Beyond the Sword\Beyond
  the Sword"

In the config block, the value for Steam is used. Change this value as described above, if needed.

### CUSTOM_ASSETS_DIR

Set CUSTOM_ASSETS_DIR to the directory Beyond the Sword uses to store custom assets.

* Replace "\*your-username\*" with your Windows username.

## Running c4recover

Once the configuration file has been updated you can run c4recover from a command line prompt using
the following command:

```
c4recover <save-name>.CivBeyondSwordSave
```
c4recover will remove LMA from the save and will write the modified save to 
recovered_\<save-name\>.CivBeyondSwordSave.

If you encounter an error while running c4recover similar to this
```
<unspecified file>: cannot open file
```
double-check your config file to ensure that values are properly set, and make sure that 
BTS.schema is in the same directory as c4recover.exe.