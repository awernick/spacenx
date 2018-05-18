#include "input.h"

#if defined(__SDL2__)
  #include <SDL2/SDL.h>
#endif

#if defined(__SWITCH__) && defined(__LIBNX__)
  #include <SDL/SDL.h>
#endif

#if defined(__SWITCH__) && !defined(__LIBNX__)
static SDL_Joystick *joystick;
#endif

void PADInit()
{
  #if defined(__SWITCH__) && !defined(__LIBNX__)
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
    if(joystick == NULL) {
      printf("Joystick init failed: %s\n", SDL_GetError());
      SDL_Quit();
      return;
    } else {
      printf("Joystick attached: %s\n", SDL_JoystickName(joystick));
    }
  #endif
}

void PADDestroy()
{
  #if defined(__SWITCH__) && !defined(__LIBNX__) // switch
    SDL_JoystickClose(joystick);
  #endif
}

void PADRead(struct PADData* data)
{
  // reset buttons
  data->btns_h = 0b00000000;
  SDL_Event event;
  SDL_PollEvent(&event);

  data->lstick_x = 0;
  data->lstick_y = 0;
  data->rstick_x = 0;
  data->rstick_x = 0;
  
  switch (event.type) {
    case SDL_KEYDOWN:
    {
      #if !defined(__SWITCH__)
        data->btns_h |= ((event.key.keysym.sym == SDLK_RETURN)?		BUTTON_A : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_BACKSPACE)?		BUTTON_B : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_UP)?		BUTTON_UP : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_DOWN)?		BUTTON_DOWN : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_LEFT)?		BUTTON_LEFT : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_RIGHT)?		BUTTON_RIGHT : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_SPACE)?		BUTTON_PLUS : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_DELETE)?		BUTTON_MINUS : 0);

        // update stick values (between -1 and 1 for each)
        data->rstick_x = -1*(event.key.keysym.sym == SDLK_w) + (event.key.keysym.sym == SDLK_s);
        data->rstick_y = -1*(event.key.keysym.sym == SDLK_a) + (event.key.keysym.sym == SDLK_d);
      #else // libnx
        data->btns_h |= ((event.key.keysym.sym == SDLK_a)?		BUTTON_A : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_b)?		BUTTON_B : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_UP)?		BUTTON_UP : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_DOWN)?		BUTTON_DOWN : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_LEFT)?		BUTTON_LEFT : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_RIGHT)?		BUTTON_RIGHT : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_PLUS)?		BUTTON_PLUS : 0);
        data->btns_h |= ((event.key.keysym.sym == SDLK_MINUS)?		BUTTON_MINUS : 0);

        // update stick values (between -1 and 1 for each)
//				data->rstick_x = -1*(event.key.keysym.sym == SDLK_w) + (event.key.keysym.sym == SDLK_s);
//				data->rstick_y = -1*(event.key.keysym.sym == SDLK_a) + (event.key.keysym.sym == SDLK_d);
      #endif
      break;
    }
    case SDL_JOYBUTTONDOWN:
    {
      data->btns_h |= ((event.jbutton.button == 0)?		BUTTON_A : 0);
      data->btns_h |= ((event.jbutton.button == 1)?		BUTTON_B : 0);
      data->btns_h |= ((event.jbutton.button == 13)?		BUTTON_UP : 0);
      data->btns_h |= ((event.jbutton.button == 15)?		BUTTON_DOWN : 0);
      data->btns_h |= ((event.jbutton.button == 12)?		BUTTON_LEFT : 0);
      data->btns_h |= ((event.jbutton.button == 14)?		BUTTON_RIGHT : 0);
      data->btns_h |= ((event.jbutton.button == 10)?		BUTTON_PLUS : 0);
      data->btns_h |= ((event.jbutton.button == 11)?		BUTTON_MINUS : 0);
      break;
    }
    case SDL_JOYAXISMOTION:
    {
      int delta = event.jaxis.value;
      switch(event.jaxis.axis) {
        case 0: // Left X-Axis
          data->lstick_x += delta / 32768.0;
          break;
        case 1: // Left Y-Axis
          data->lstick_y += delta / 32768.0;
          break;
        case 2: // Right X-Axis
          data->rstick_x += delta / 32768.0;
          break;
        case 3: // Right Y-Axis
          data->rstick_y += delta / 32768.0;
          break;
      }
    }
  }
}