cmake_minimum_required(VERSION 3.22)
project(WhiteNoisePOC VERSION 0.1.0)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 9.0.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(WhiteNoise
    COMPANY_NAME "POC Audio"
    BUNDLE_ID "com.pocaudio.whitenoise"
    PLUGIN_MANUFACTURER_CODE PocA
    PLUGIN_CODE WN01
    PRODUCT_NAME "White Noise"
    FORMATS VST3
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Instrument Synth
)

juce_generate_juce_header(WhiteNoise)

target_sources(WhiteNoise PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
)

target_compile_features(WhiteNoise PRIVATE cxx_std_17)
target_compile_definitions(WhiteNoise PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(WhiteNoise PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
