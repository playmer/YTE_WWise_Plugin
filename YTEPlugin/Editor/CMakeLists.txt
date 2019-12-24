add_library(YTE_Editor_WWise_Plugin SHARED "")

target_sources(YTE_Editor_WWise_Plugin
  PRIVATE  
    EditorWWisePlugin.cpp
    WWiseWidget.cpp
    WWiseWidget.hpp
    WWiseEditorMeta.hpp
)

set_target_properties(YTE_Editor_WWise_Plugin
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${YTEPlugins_Library_Dir}
                      LIBRARY_OUTPUT_DIRECTORY ${YTEPlugins_Library_Dir}
                      RUNTIME_OUTPUT_DIRECTORY ${YTEditorPluginsDirectory})

target_link_libraries(YTE_Editor_WWise_Plugin 
  PUBLIC
    YTE
    YTEditor
    YTEPlugin_WWise
)

target_compile_definitions(YTE_Editor_WWise_Plugin PRIVATE YTEditorPlugin_WWise_Internal=1)
