target_sources(app PRIVATE
    gui.c
    events_handler.c
)

add_subdirectory(widgets)
add_subdirectory(screens)
