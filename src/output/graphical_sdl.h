#define SDL_DISPLAY_NUM 2
#define SDL_DISPLAY_NAME "sdl"

void cleanup_graphical_sdl(void);
int init_window_sdl(void);
void clear_screen_sdl(void);
int apply_window_settings_sdl(void);
int get_window_input_sdl(void);
void draw_graphical_sdl(int bars, int rest, int f[200], int flastd[200]);
