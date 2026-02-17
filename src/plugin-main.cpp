#include <obs-module.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <obs-frontend-api.h>

#ifdef __cplusplus
}
#endif

#include <QMainWindow>
#include <QDockWidget>
#include "plugin-macros.generated.h"
#include "obs-text-slideshow-dock.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

extern void load_text_freetype2_slideshow();
#ifdef _WIN32
extern void load_text_gdiplus_slideshow();
#endif

bool obs_module_load(void)
{
    load_text_freetype2_slideshow();
#ifdef _WIN32
    load_text_gdiplus_slideshow();
#endif

    const auto main_window =
        static_cast<QMainWindow *>(obs_frontend_get_main_window());
    
    if (main_window) {
        obs_frontend_push_ui_translation(obs_module_get_string);
        
        auto *tmp = new TextSlideshowDock(main_window);
        
        obs_frontend_add_custom_qdock("TextSlideshow", static_cast<void*>(tmp));
        
        obs_frontend_pop_ui_translation();
    }

    blog(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);
    
    return true;
}

void obs_module_unload()
{
    blog(LOG_INFO, "plugin unloaded");
}
