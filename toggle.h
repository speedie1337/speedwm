/* Toggle patches
 * This header allows you to enable/disable patches that can break compability or decrease performance with certain OSes or configurations.
 *
 * If it's disabled (set to 0), it will not be compiled in with speedwm. This keeps the binary small and may make the experience better.
 */

/* IPC */
#define USEIPC           1 /* Include IPC.
NOTE: If you set this to 1, set  USEIPC to true in toggle.mk and comment the YAJLLIBS and YAJLINC lines in config.mk.
Not compatible with BSDs so for those, set this to 0. */

/* Alpha related patches
 *
 * These do not have any dependencies but can slow down older computers.
 * Because of this, you can easily toggle them on/off.
 */
#define USEALPHA         1 /* Include transparency for the bar */
#define USEFADE          1 /* Include fading windows */

/* Miscellanious */
#define USEQUERY         1 /* Include support for queries */
#define USESWITCHER      1 /* Include the switcher */
#define USESYSTRAY       1 /* Include the systray */
#define USEMOUSE         1 /* Include mouse binds */
#define USEXRESOURCES    1 /* Include .Xresources support. */

/* Image related patches
 *
 * These can slow down speedwm slightly but if you include modern computers, it's not noticeable.
 * If you wish to disable them though, set them to 0.
 */
#define USEIMLIB2        1 /* Include imlib2. Required by USEWINICON and USETAGPREVIEW. */
#define USEWINICON       1 /* Include window icons. Requires imlib to be enabled in toggle.mk and it must be installed. */
#define USETAGPREVIEW    1 /* Include tag previews. Requires imlib to be enabled in toggle.mk and it must be installed. */
