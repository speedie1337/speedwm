## 12. Keybinds

Like the bar array mentioned previously, there is a `Key keys` array in `keybinds.h` which contains all keybinds speedwm will recognize. While this isn't the only way to add keybinds in speedwm, it does not require any additional software to be installed.

### Event

The event value allow you to specify when a keybind is executed.

- KeyPress:               Activate immediately on key press.
- KeyRelease:             Activate immediately on key release.

### Modifier

There are a lot of different modifiers, this list is only going to list the ones declared in speedwm.

- CONTROL                 Left Control (Ctrl) key.
- SHIFT                   Left Shift key.
- ALT                     Left Alt key.
- SUPER                   Left Super (Windows) key.
- SUPERR                  Right Super (Windows) key.

In keybinds.h, MODIFIER1 and MODIFIER2 are also defined. MODIFIER1 is defined in order to make modifying keybinds easier. MODIFIER1 is by default defined as SUPER or Mod4Mask. For instance, if you want to use MODIFIER1 as your modifier, `MODIFIER1` would be the right value here.

Do note that you can may use multiple modifiers. Do this by adding two or more modifiers separated by a pipe (MODIFIER1|SHIFT)

### Chain key

This value *is* mandatory, but it does not have to be used. speedwm has supported chained keybinds since 0.4. Chained keybinds allow more keybinds as like the name implies, the user has to press multiple keys to activate it. You can think of it as in Vim where there's different modes.

The first key that needs to be pressed to activate a chain needs to be the key specified here. In order to see a list of possible keys, see `/usr/include/X11/keysymdef.h`. If, let's say we want to use simply the letter `a` as the key to start a chain, the value here would be `XK_a`.

If we want a normal key, one that is **not** chained, we can accomplish that by simply setting the Chain key to `-1`.

### Key

The next value, similar to the Chain key also needs to be a keysym. If the key is a chained key, this is the second key that needs to be pressed. If it isn't a chained key, this is the key that needs to be pressed while the Modifier is pressed.

In order to see a list of possible keys, see `/usr/include/X11/keysymdef.h`. Note that this path may differ depending on your operating system.

There are a lot more keys technically defined, see `/usr/include/X11/XF86keysym.h` for a list of media keys.

### Function

Many functions expect an argument to be passed to them. However this value does require any function argument to be specified because it simply passes the next value to the function and calls it.

There is no list of functions, but `speedwm.c` has a lot of declared functions which you may use in keybinds provided the function expects `const Arg *arg`.

### Function arguments

This is the actual argument passed to the function we're calling when the keybind is activated.

Passing a float (such as mfact) through would mean I should override `.f`, passing an integer (such as barposition) through would mean I should override `.i`. There's also `.v` for `void` and `.ui` for tags. This is not used unless you want to mess with tag keybinds.

When you don't care about the value given to the function, you may simply enter a `0`.

### Example keybind

With all that said, you probably get the idea of how it works. But you still don't know how to put all this knowledge together.

1. `{ KeyPress,`
2. `MODIFIER1|SHIFT,`
3. `-1,`
4. `XK_w,`
5. `spawn,`
6. `cmd( "firefox" ) },`

Combined into one line: `{ KeyPress, MODIFIER1|SHIFT, -1, XK_w, spawn, cmd( "firefox" ) },`

### Example chained keybind

1. `{ KeyPress,`
2. `MODIFIER1|SHIFT,`
3. `XK_a,`
4. `XK_w,`
5. `togglebar,`
6. `{0} },`

Combined into one line: `{ KeyPress, MODIFIER1|SHIFT, XK_a, XK_w, togglebar, {0} },`

This should give you a brief idea of how keybinds work in speedwm. Most of this carries over to `dwm` as well, although it does not have some of this stuff.

