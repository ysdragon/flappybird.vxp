#include "common.h"
#include "pixels.h"

VMINT game_timer = -1;

void reset_game(void)
{
    bird_y = SCREEN_HEIGHT / 2;
    bird_vel = 0;
    score = 0;
    game_over = VM_FALSE;

    for (int i = 0; i < NUM_PIPES; i++)
    {
        pipe_x[i] = SCREEN_WIDTH + i * PIPE_SPACING;
        pipe_gap_y[i] = rand() % (SCREEN_HEIGHT - PIPE_GAP - 2 * GROUND_HEIGHT) + GROUND_HEIGHT;
    }
}

void timer_callback(VMINT tid)
{
    if (game_over || layer_hdl[0] == -1)
    {
        if (game_over)
            draw_game();
        return;
    }

    bird_vel += GRAVITY;
    bird_y += bird_vel;

    int scaled_width = (int)(BIRD_WIDTH * BIRD_SCALE);
    int scaled_height = (int)(BIRD_HEIGHT * BIRD_SCALE);

    if (bird_y + (scaled_height / 2) > SCREEN_HEIGHT - GROUND_HEIGHT || bird_y - (scaled_height / 2) < 0)
    {
        game_over = VM_TRUE;
        draw_game();
        return;
    }

    for (int i = 0; i < NUM_PIPES; i++)
    {
        pipe_x[i] -= PIPE_SPEED;

        if (pipe_x[i] < BIRD_X + (scaled_width / 2) && pipe_x[i] + PIPE_WIDTH > BIRD_X - (scaled_width / 2))
        {
            if (bird_y - (scaled_height / 2) < pipe_gap_y[i] || bird_y + (scaled_height / 2) > pipe_gap_y[i] + PIPE_GAP)
            {
                game_over = VM_TRUE;
                draw_game();
                return;
            }
        }

        if (pipe_x[i] + PIPE_WIDTH < BIRD_X && pipe_x[i] + PIPE_WIDTH + PIPE_SPEED >= BIRD_X)
        {
            score++;
        }

        if (pipe_x[i] + PIPE_WIDTH < 0)
        {
            pipe_x[i] = pipe_x[(i + NUM_PIPES - 1) % NUM_PIPES] + PIPE_SPACING;
            pipe_gap_y[i] = rand() % (SCREEN_HEIGHT - PIPE_GAP - 2 * GROUND_HEIGHT) + GROUND_HEIGHT;
        }
    }

    draw_game();
}

void draw_game(void)
{
    if (layer_hdl[0] == -1)
        return;

    vm_graphic_color color;
    VMWCHAR score_str[128];
    char score_char_buf[10];

    color.vm_color_565 = VM_COLOR_BLUE;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    color.vm_color_565 = VM_COLOR_GREEN;
    vm_graphic_setcolor(&color);
    vm_graphic_fill_rect_ex(layer_hdl[0], 0, SCREEN_HEIGHT - GROUND_HEIGHT, SCREEN_WIDTH, GROUND_HEIGHT);

    color.vm_color_565 = VM_COLOR_GREEN;
    vm_graphic_setcolor(&color);
    for (int i = 0; i < NUM_PIPES; i++)
    {
        vm_graphic_fill_rect_ex(layer_hdl[0], pipe_x[i], 0, PIPE_WIDTH, pipe_gap_y[i]);
        vm_graphic_fill_rect_ex(layer_hdl[0], pipe_x[i], pipe_gap_y[i] + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (pipe_gap_y[i] + PIPE_GAP) - GROUND_HEIGHT);
    }

    int scaled_width = (int)(BIRD_WIDTH * BIRD_SCALE);
    int scaled_height = (int)(BIRD_HEIGHT * BIRD_SCALE);

    for (int y = 0; y < scaled_height; y++)
    {
        for (int x = 0; x < scaled_width; x++)
        {
            int orig_x = x * BIRD_WIDTH / scaled_width;
            int orig_y = y * BIRD_HEIGHT / scaled_height;

            VMUINT16 pixel = bird_pixels[orig_y * BIRD_WIDTH + orig_x];
            if (pixel != 0xFFFF)
            {
                color.vm_color_565 = pixel;
                vm_graphic_setcolor(&color);
                vm_graphic_set_pixel_ex(layer_hdl[0], BIRD_X - (scaled_width / 2) + x, bird_y - (scaled_height / 2) + y);
            }
        }
    }

    sprintf(score_char_buf, "%d", score);
    vm_gb2312_to_ucs2(score_str, 20, (VMSTR)score_char_buf);
    color.vm_color_565 = VM_COLOR_BLACK;
    vm_graphic_setcolor(&color);
    vm_graphic_textout_to_layer(layer_hdl[0], 10, 10, score_str, vm_wstrlen(score_str));

    if (game_over)
    {
        vm_gb2312_to_ucs2(score_str, 128, (VMSTR) "Game Over!");
        int text_w = vm_graphic_get_string_width(score_str);
        int text_h = vm_graphic_get_character_height();
        int x = (SCREEN_WIDTH - text_w) / 2;
        int y = SCREEN_HEIGHT / 2 - text_h;

        color.vm_color_565 = VM_COLOR_RED;
        vm_graphic_setcolor(&color);
        vm_graphic_textout_to_layer(layer_hdl[0], x, y, score_str, vm_wstrlen(score_str));

        vm_ascii_to_ucs2(score_str, 128, (VMSTR) "Press any key to restart.");
        text_w = vm_graphic_get_string_width(score_str);
        x = (SCREEN_WIDTH - text_w) / 2;
        y += text_h + 5;
        color.vm_color_565 = VM_COLOR_BLACK;
        vm_graphic_setcolor(&color);
        vm_graphic_textout_to_layer(layer_hdl[0], x, y, score_str, vm_wstrlen(score_str));
    }

    /* flush the screen with drawn data */
    vm_graphic_flush_layer(layer_hdl, 1);
}