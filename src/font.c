#include "font.h"

static GLuint fnt_texture_id;
static int fnt_sprite_width;
static int fnt_sprite_height;
static int fnt_image_width;
static int fnt_image_height;
static struct fnt {
    double tex_start_x;
    double tex_start_y;
    double tex_end_x;
    double tex_end_y;
} fonts[95];


static void font_precompute();

void font_init(GLuint texture_id, int sprite_width, int sprite_height, int image_width, int image_height)
{
    d("[font] init\n");

    fnt_texture_id = texture_id;
    fnt_sprite_width = sprite_width;
    fnt_sprite_height = sprite_height;
    fnt_image_width = image_width;
    fnt_image_height = image_height;

    d("[font] starting pre-compute at 32\n");
    font_precompute();
    d("[font] done\n");
}

static void font_precompute()
{
    double tex_start_x = 0;
    double tex_start_y = 0;

    double tex_step_x = 32;
    double tex_step_y = 64;

    double tex_end_x = 0 + tex_step_x;
    double tex_end_y = 0 + tex_step_y;

    for (int i = 32; i < 127; i++) {
        unsigned int index = i - 32;

        // we're inverting Y
        fonts[index].tex_start_x = tex_start_x / (double)fnt_image_width;
        fonts[index].tex_start_y = 1 - tex_end_y / (double)fnt_image_height;
        fonts[index].tex_end_x = tex_end_x / (double)fnt_image_width;
        fonts[index].tex_end_y = 1 - tex_start_y / (double)fnt_image_height;
        d("[font] %2d %c - %6.4lf %6.4lf -> %6.4lf %6.4lf\n", index, i, fonts[index].tex_start_y, fonts[index].tex_start_x, fonts[index].tex_end_y, fonts[index].tex_end_x);
        tex_start_x += tex_step_x;
        tex_end_x += tex_step_x;

        if (tex_end_x > fnt_image_width) {
            tex_start_x = 0;
            tex_end_x = 0 + tex_step_x;

            tex_start_y += tex_step_y;
            tex_end_y += tex_step_y;
        }
    }

    // TODO: precomputed lists...
}

void font_draw(double y, double x, int size, const char *fmt, ...)
{
    // to buffer

    static char buffer[SPRINTF_BUFFER_SIZE];

    va_list va;
    va_start(va, fmt);

    int rc = vsnprintf(buffer, SPRINTF_BUFFER_SIZE, fmt, va);
    if (rc == -1 || rc >= SPRINTF_BUFFER_SIZE) {
        fprintf(stderr, "[font_draw]: Buffer overflow: %s", fmt);
        exit(1);
    }

    // write

    if (size == 0) size = 16;
    double font_size_width = size / 1024.0;
    double font_size_height = font_size_width * 2;
    double draw_x = x;
    double draw_y = y;

    glBindTexture(GL_TEXTURE_2D, fnt_texture_id);

    for (unsigned int ord = 0; buffer[ord]; ord++) {

        glBegin(GL_QUADS);

        unsigned char index = buffer[ord] - 32;

        glTexCoord2d(fonts[index].tex_start_x, fonts[index].tex_end_y);
        glVertex2d(draw_x, draw_y);
        
        glTexCoord2d(fonts[index].tex_start_x, fonts[index].tex_start_y);
        glVertex2d(draw_x, draw_y + font_size_height);
        
        glTexCoord2d(fonts[index].tex_end_x, fonts[index].tex_start_y);
        glVertex2d(draw_x + font_size_width, draw_y + font_size_height);

        glTexCoord2d(fonts[index].tex_end_x, fonts[index].tex_end_y);
        glVertex2d(draw_x + font_size_width, draw_y);

        glEnd();

        draw_x += font_size_width;
    }

    glBindTexture(GL_TEXTURE_2D, 0);    

}
