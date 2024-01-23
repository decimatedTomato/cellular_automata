#pragma once

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080

void take_user_input();
void init_Uniforms();
void init_Shader(char *vertex_filepath, char *fragment_filepath);
void init_Quad();
void init_Debug_Callback();
int init_GLFW(int width, int height, char* name);

// void load_image_texture(int slot);
void init_Texture(const void *pixels, int grid_width, int grid_height);
void update_Texture(const void *pixels, int grid_width, int grid_height);
void clean_up();
bool render_frame();