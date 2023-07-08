cmake_minimum_required(VERSION 3.15)

project(DVA_MANAGER VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

#find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
add_subdirectory("../../JUCE" JUCE)     # If you've put JUCE in a subdirectory called JUCE

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (DvaManager here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_plugin(DvaManager
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME maiken                         # Specify the name of the plugin's author
        # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
        # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE MAPC               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE P006                            # A unique four-character plugin id with exactly one upper-case character
        # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
        FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME "DVA Manager")                 # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header(DvaManager)

target_sources(DvaManager
        PRIVATE
        source/PluginEditor.cpp
        source/PluginProcessor.cpp
        source/Analyser/Analyser.cpp
        source/Analyser/Canvas.cpp
        source/Analyser/FFTBuffer.cpp
        source/Analyser/ResponseCurve.cpp
        source/Analyser/Spectrogram.cpp
        source/Controls/ControlsContainer.cpp
        source/Controls/Compressor/CompressorControls.cpp
        source/Controls/EQ/EqControls.cpp
        source/Controls/EQ/CutBand.cpp
        source/Controls/EQ/PeakBand.cpp
        source/Controls/EQ/SlopeGroup.cpp
        source/FileInfo/CustomGridCell.cpp
        source/FileInfo/FileInfo.cpp
        source/FilterDsp/FilterDsp.cpp
        source/FolderView/FolderButton.cpp
        source/FolderView/FolderSelector.cpp
        source/LUFS/LufsFilter.cpp
        source/MainView/MainView.cpp
        source/Menu/DvaListBox.cpp
        source/Meter/Meter.cpp
        source/Meter/MeterGroup.cpp
        source/Meter/MeterValueLabel.cpp
        source/Meter/ValueHolder.cpp
        source/Transport/TransportBar.cpp
        source/Transport/TransportButton.cpp
        source/Utilities/GlobalLookAndFeel.cpp
        source/Waveform/GraphicWaveform.cpp
        source/Waveform/PositionMarker.cpp
        source/Waveform/WaveformContainer.cpp
        source/Widgets/AnalyserNode.cpp
        source/Widgets/CompRotary.cpp
        source/Widgets/DbMarkers.cpp
        source/Widgets/EnablementButton.cpp
        source/Widgets/EqRotary.cpp
        source/Widgets/FileButton.cpp
        source/Widgets/FreqMarkers.cpp
        source/Widgets/PowerButton.cpp
        source/Widgets/RadioButton.cpp
        source/Widgets/RotaryBase.cpp
        source/Widgets/ViewButton.cpp
        )

target_compile_definitions(DvaManager
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(AudioPluginData SOURCES
        resources/file.png
        resources/folder.png
        resources/loop.png
        resources/pause.png
        resources/play.png
        resources/power.png
        resources/stop.png)

target_link_libraries(DvaManager
        PRIVATE
        AudioPluginData
        juce::juce_audio_basics
        juce::juce_audio_utils
        juce::juce_dsp
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
