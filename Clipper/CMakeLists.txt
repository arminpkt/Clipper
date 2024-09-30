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
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
