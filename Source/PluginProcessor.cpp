/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BitCrusherAudioProcessor::BitCrusherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    :apvst(*this, nullptr, "PARAMETER", createParameterLayout())
{
}

BitCrusherAudioProcessor::~BitCrusherAudioProcessor()
{
}

//==============================================================================
const String BitCrusherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BitCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BitCrusherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BitCrusherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BitCrusherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BitCrusherAudioProcessor::setCurrentProgram (int index)
{
}

const String BitCrusherAudioProcessor::getProgramName (int index)
{
    return {};
}

void BitCrusherAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BitCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BitCrusherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BitCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BitCrusherAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    updateParameters();

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int bitMultiplier = pow(2, bitDepth) - 1;

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            channelData[sample] = processSample(channelData[sample], bitMultiplier);
        }

        if (reductionType != 0)
        {
            int bitRate = pow(2, sampleRateReduction);
            int step = getSampleRate() / bitRate;

            float holdSample = channelData[0];

            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                if ((sample + step) % step == 0)
                {
                    holdSample = channelData[sample];
                }

                channelData[sample] = holdSample;
            }
        }
    }
}

//==============================================================================
bool BitCrusherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BitCrusherAudioProcessor::createEditor()
{
    return new BitCrusherAudioProcessorEditor (*this);
}

//==============================================================================
void BitCrusherAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BitCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitCrusherAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout BitCrusherAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> parameters;

    auto bitDepthParam = std::make_unique<AudioParameterFloat>("bit depth", "Bit Depth", 1.f , 16.f, 16.f);
    parameters.push_back(std::move(bitDepthParam));

    auto sampleRateparam = std::make_unique<AudioParameterFloat>("sample rate", "Sample Rate", 1.f, 16.f, 1.f);
    parameters.push_back(std::move(sampleRateparam));

    auto blendParam = std::make_unique<AudioParameterFloat>("blend", "Blend", 0.f, 1.f, 1.f);
    parameters.push_back(std::move(blendParam));

    auto masterGainParam = std::make_unique<AudioParameterFloat>("master gain", "Mater Gain", -60.f, 0.f, 0.f);
    parameters.push_back(std::move(masterGainParam));

    auto reductionTypeParam = std::make_unique<AudioParameterFloat>("reduction type", "Reduction Type", 0.f, 1.f, 0.f);
    parameters.push_back(std::move(reductionTypeParam));

    return { parameters.begin(), parameters.end() };
}

void BitCrusherAudioProcessor::updateParameters()
{
    bitDepth = *(apvst.getRawParameterValue("bit depth"));
    sampleRateReduction = *(apvst.getRawParameterValue("sample rate"));
    blend = *(apvst.getRawParameterValue("blend"));
    masterGain = *(apvst.getRawParameterValue("master gain"));
    reductionType = *(apvst.getRawParameterValue("reduction type"));
}

float BitCrusherAudioProcessor::processSample(float sample, int bitMultiplier)
{
    float effectedSample = sample;

    if (reductionType == 0)
    {
        effectedSample = (float)(ceil((effectedSample + 1.0) * bitMultiplier));
        effectedSample = effectedSample / bitMultiplier - 1.0;
    }

    sample = (effectedSample * blend) + (sample * (1 - blend));

    return sample * Decibels::decibelsToGain(masterGain);
}
