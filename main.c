// test_comic.c
#include "comic/comic.h"
#include "engine/engine.h"
#include <stdio.h>

static Comic* g_comic = NULL;

void engine_init() {
    printf("Initializing Comic Engine...\n");
    
    // 初始化渲染器、输入等
    renderer_init(800, 600, "Comic Engine Demo");
    input_init();
    
    // 加载漫剧
    g_comic = comic_load("assets/comics/prologue.json");
    if (!g_comic) {
        printf("Failed to load comic!\n");
    }
}

void engine_update(float delta_time) {
    // 处理输入
    input_update();
    
    if (g_comic) {
        // 更新漫剧
        comic_update(g_comic, delta_time);
        
        // 检测下一页
        if (input_key_pressed(KEY_SPACE) || input_mouse_button_pressed(MOUSE_LEFT_BUTTON)) {
            comic_next_panel(g_comic);
        }
    }
    
    // 退出检测
    if (input_key_pressed(KEY_ESCAPE)) {
        engine_should_close();
    }
}

void engine_render() {
    // 开始渲染
    renderer_clear((Color){20, 20, 30, 255});
    
    if (g_comic) {
        // 渲染漫剧
        comic_render(g_comic);
    }
    
    // 显示FPS
    char fps_text[32];
    snprintf(fps_text, sizeof(fps_text), "FPS: %d", (int)engine_get_fps());
    font_draw_text(font_get_default(), fps_text, 10, 10, 20, (Color){255, 255, 255, 200});
    
    // 结束渲染
    renderer_present();
}

void engine_cleanup() {
    if (g_comic) {
        comic_free(g_comic);
        g_comic = NULL;
    }
    
    renderer_shutdown();
}

int main() {
    engine_init();
    
    // 主循环
    while (!engine_should_close()) {
        float delta_time = engine_get_delta_time();
        
        engine_update(delta_time);
        engine_render();
        
        engine_sleep(16); // ~60 FPS
    }
    
    engine_cleanup();
    return 0;
}