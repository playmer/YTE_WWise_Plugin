add_library(YTEPlugin_WWise SHARED "")

target_sources(YTEPlugin_WWise
  PRIVATE  
    ForwardDeclarations.hpp
    MemoryImplementation.cpp
    Utilities.hpp
    WWiseMeta.hpp
    WWisePlugin.cpp
    WWiseEmitter.cpp
    WWiseEmitter.hpp
    WWiseListener.cpp
    WWiseListener.hpp
    WWiseSystem.cpp
    WWiseSystem.hpp
    WWiseView.hpp
    WWiseView.cpp
)

set_target_properties(YTEPlugin_WWise
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY ${YTEPlugins_Library_Dir}
                      LIBRARY_OUTPUT_DIRECTORY ${YTEPlugins_Library_Dir}
                      RUNTIME_OUTPUT_DIRECTORY ${YTEPluginsDirectory})

target_link_libraries(YTEPlugin_WWise PUBLIC WWise_SoundEngine YTE)

target_include_directories(YTEPlugin_WWise PUBLIC ${YTE_Plugin_WWise_Root})
target_compile_definitions(YTEPlugin_WWise PRIVATE YTEPlugin_WWise_Internal=1)
