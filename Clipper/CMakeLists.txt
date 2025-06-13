project(Clipper VERSION 0.0.1)

juce_add_plugin(Clipper
    VERSION 0.0.1                              
    # ICON_BIG ...                              
    # ICON_SMALL ...
    COMPANY_NAME Armin                         
    # IS_SYNTH TRUE/FALSE                       
    # NEEDS_MIDI_INPUT TRUE/FALSE               
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              
    # IS_MIDI_EFFECT TRUE/FALSE                 
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        
    PLUGIN_MANUFACTURER_CODE AP44               
    PLUGIN_CODE Clpr                            
      
    FORMATS AU VST3 Standalone                  
    PRODUCT_NAME "Clipper")

#juce_add_binary_data(ClipperBinaryData SOURCES Resources/clipperbg.png Resources/clipperbg2.png Resources/clipperknob1.png Resources/clipperknob2.png Resources/PlusJakartaSans_Medium.ttf)
juce_add_binary_data(ClipperBinaryData SOURCES Resources/clipperbg.png Resources/clipperbg2.png Resources/clipperknob1.svg Resources/clipperknob2.svg Resources/PlusJakartaSans_Medium.ttf)

target_sources(Clipper
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp)

target_compile_definitions(Clipper
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Clipper
    PRIVATE
        juce::juce_audio_utils
        ClipperBinaryData
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

#set(VST3_DESTINATION "/Library/Audio/Plug-Ins/VST3/")
#set(AU_DESTINATION "/Library/Audio/Plug-Ins/Components/")

# Copy VST3 and AU files upon build completion
#add_custom_command(TARGET Clipper POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        "$<TARGET_FILE_DIR:Clipper>/Clipper.vst3"
#        "${VST3_DESTINATION}/Clipper.vst3"
#        COMMAND ${CMAKE_COMMAND} -E copy
#        "$<TARGET_FILE_DIR:Clipper>/Clipper.component"
#        "${AU_DESTINATION}/Clipper.component"
#)