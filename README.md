# mechware
Modular and configurable plugin based firmware for mechanical keyboard.

There are 3 different types of plugins, input, wire, and general plugins. The plugins are registered on startup, and run sequentially in the specified order, each one returning a boolean value indicating whether to terminate the chain at that point.

**input/** contains device input classes, currently one which performs the scan of the keyboard matrix and does press/release detection and debounce. Further input classes might include I2C or Serial input classes, responsible for reading values from the other half of a split keyboard for example, and inserting them into the chain.

The Input classes call into the KeyboardState class which translates the row/column into a scan code by looking up the relevent Key Map. This scan code is used to call the scan code plugins. These are initialised with one or more scancodes, and can check the those scan codes during the plugin execution to determine functionality.

**plugin/** contains a variety of functional plugin types, each gets called in turn with key events passed up from the input/ plugin chain, and can perform operations. 
Currently contains the following functional plugins, 
* FnPlugin : handles Fn(s) key on my current board, pressing the key raises the current Keymap level (currently there are only two in my config), releasing the key lowers it (to the base level again). A similar 'LevelPlugin' could be used to handle two keys, to raise and lower through arbitrary key map levels.
* SticKeyPlugin : checks for double clicks on specific keys, and if registered, will return 'false' to swallow the relevent 'KEY_RELEASE' event, those resulting in the key never being un pressed. I have it setup on Fn keys and the left Shift key to allow me to toggle the keyboard into the Function layer without having to hold the Fn key down, and on the left shift to approximate the Capslock (which has been replaced by another Fn key on the left)
* MacroPlugin : If held down, will record up to 100 keystrokes, returning false as it does so, then if 'clicked' will replay those keystrokes. Holding it down for more than 2 seconds without pressing anything else will clear the current buffer. This is a good example of some fairly sophisticated time based functionality in a plugin. Also a bit of a WIP, at the moment it will only handle 1 key as the macro key correctly.

**wire/** if all the scan code plugins return true, the wire plugin chain is called. There are two at the moment, 
 * USBWireHandler: outputs key press and release events on USB so the keyboard can function as, well, a keyboard.
 * SerialWireHandler I was using this as an alternative to the USB handler while debugging, it's useful sometimes when your keyboard-in-progress DOESN'T act like a keyboard, when you screw stuff up.

**TODO** 
* Setup a proper build system and move off the Arduino framework. At the moment I'm relying on Platform IO and its vscode plugin to build. Probably also involves writing some base level HID classes to replace the Arduino 'Keyboard' class which I'm currently using.
* Find a better way of encapsulating the config somehow, at the moment it's defined in the main.cpp file and the pin configuration in the MatrixScan input device class (where, arguably, it belongs of course)
* Currently if the plugins want to do something that involves pressing a key they have to call the wirehandler chain directly with the scancodes and key state. They should instead be sticking them onto an existing queue of keys, all of which are sent through the scan code plugins. This would facilitate some other behaviours around timing that would be useful. 
* Allow chords to trigger plugins i.e. CTRL+ALT+P might printscreen etc etc

---

Originally put together with vscode and its platformio plugin. Current Keyboard has a Teensy LC brain, Relevent platformio.ini is included in the repo.

Initial design and simple firmware for the keyboard is here https://github.com/dairequinlan/the-keyboard 

Build log and description here https://www.dairequinlan.com/tags/the-keyboard/
