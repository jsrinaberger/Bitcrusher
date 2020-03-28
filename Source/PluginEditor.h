

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BitCrusherAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BitCrusherAudioProcessorEditor (BitCrusherAudioProcessor&);
    ~BitCrusherAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BitCrusherAudioProcessor& processor;

    Slider bitDepthSlider;
    Slider sampleRateSlider;
    Slider blendSlider;
    Slider masterGainSlider;

    ToggleButton bitReductionToggleButton{ "Bit Reduction" };
    ToggleButton downSampleToggleButton{ "Down Sample" };

    void bitReductionButtonClicked();
    void downSampleButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherAudioProcessorEditor)

public:

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> bitDepthValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sampleRateValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> blendValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> masterGainValue;
};
