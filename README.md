### Attempt to use SDL2

Notes in here are not meant to be usefull for anyone but me.
I like to put some notes which helps me remember stuff, even though I never
read them again. But feel free to read it and modify it if you like it.
But do not expect much out of it.

## Errors
1. Code seems to be working but not window visible!
Solution:
    Instead of using `SDL_Delay(500)` which was show in example codea,
    create event_hanlder and handle buttons presses;
    For somereason SDL_Delay does not let SDL to display window properly,
    even if it can be visible in task bar.

2. When program is building properly but images is not displayed!
Solution:
    Make sure that path to the "assets" is correct in relation to executable file.
    For file structure like this:
    ```
        project
        | assets
        | build
          | exec
    ```
    Path should be `.../assets/file.bmp` oterwise `exec` will not be able 
    properly locate the file.

3. Image getting is not displayed properly for sprite animations!
    Make sure its is an correct one
    Source rect and dest rect should be the same size

4. Incomplete filed in struct
   That may means filed should be a pointer and adding * to it should help
   Ex.
   ```c
   typedef struct {
       SDL_Texture field; // this is wrong, it should be SDL_Texture * filed
   } Name;

5. When calling method from different file does not work!
    Make sure that the value passed to the fucntion as passed as references/pointer
    not as a value. So the C lang can update it. Becuase when passing it as value
    C create copy entire structure so changes made to it does not effect it 
    externaly.
6. Just not for myself: INITIALIZE YOUR POINTERS ALWAYS!
    This does not exactly mean ALWAYS as ALWAYS. It mean if the pointer will be
    used should be initialized, other way you will have find reason for 
    'segmentation fault' which is not easy if codebase is big.
    But when pointers are not used make sure you ignore them, something like:
    ```c
    if (pointer == NULL) {
        // do stuff here
    }
    ```
7. If jostick on controller does not respons properly (continuesly)!
    Instead of using `SDL_JOYAXISMOTION` which actually work only when you
    move you joystick constantly. This event is not detected when 
    joystick is hold down, use `SDL_JoystickGetAxis(joy, stick)`.
    This method constatly ask joy which direction is moved and then
    return value between -37000~ and 37001~ so you can check in update
    method if that value is greater then some `DEADZONE` and than continue
    with game.

NOTE!!!!!!!!!!!!!!!
L2 and R2 on PS4 controller are AXIS not a BUTTONS on SDL
