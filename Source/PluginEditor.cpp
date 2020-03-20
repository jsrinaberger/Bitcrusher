

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BitCrusherAudioProcessorEditor::BitCrusherAudioProcessorEditor (BitCrusherAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 150);


    bitDepthValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvst, "bit depth", bitDepthSlider);

    bitDepthSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    bitDepthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    bitDepthSlider.setTextBoxIsEditable(true);
    bitDepthSlider.setRange(1, 16, 1.f);
    bitDepthSlider.setValue(16);
    addAndMakeVisible(&bitDepthSlider);


    sampleRateValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvst, "sample rate", sampleRateSlider);

    sampleRateSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    sampleRateSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    sampleRateSlider.setTextBoxIsEditable(true);
    sampleRateSlider.setRange(1, 100, 1.f);
    sampleRateSlider.setValue(1);
    addAndMakeVisible(&sampleRateSlider);


    blendValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvst, "blend", blendSlider);

    blendSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    blendSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    blendSlider.setTextBoxIsEditable(true);
    blendSlider.setRange(0.f, 1.f, .001f);
    blendSlider.setValue(1.f);
    addAndMakeVisible(&blendSlider);


    masterGainValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvst, "master gain", masterGainSlider);

    masterGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    masterGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    masterGainSlider.setTextBoxIsEditable(true);
    masterGainSlider.setRange(-60.f, 0.f, 0.01f);
    masterGainSlider.setValue(0.f);
    addAndMakeVisible(&masterGainSlider);


    reductionTypeButton.setToggleState(false, dontSendNotification);
    reductionTypeButton.onClick = [&]() { reductionTypeButtonClicked(); };
    addAndMakeVisible(&reductionTypeButton);
}

BitCrusherAudioProcessorEditor::~BitCrusherAudioProcessorEditor()
{
}

//==============================================================================
void BitCrusherAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    reductionTypeButton.setBounds(getWidth() / 2 - 250, getHeight() / 2 - 50, 100, 100);
    bitDepthSlider.setBounds(getWidth() / 2 - 150, getHeight() / 2 - 50, 100, 100);
    sampleRateSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
    blendSlider.setBounds(getWidth() / 2 + 50, getHeight() / 2 - 50, 100, 100);
    masterGainSlider.setBounds(getWidth() / 2 + 150, getHeight() / 2 - 50, 100, 100);
}

void BitCrusherAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void BitCrusherAudioProcessorEditor::reductionTypeButtonClicked()
{
    auto* reductionTypeParam = processor.apvst.getParameter("reduction type");

    if (*(processor.apvst.getRawParameterValue("reduction type")) == 0)
    {
        reductionTypeParam->beginChangeGesture();
        reductionTypeParam->setValueNotifyingHost(1.f);
        reductionTypeParam->endChangeGesture();
    }
    else
    {
        reductionTypeParam->beginChangeGesture();
        reductionTypeParam->setValueNotifyingHost(0.f);
        reductionTypeParam->endChangeGesture();
    }
}
