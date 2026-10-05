#include "PluginProcessor.h"

namespace
{
constexpr auto levelParameterId = "level";
}

WhiteNoiseAudioProcessor::WhiteNoiseAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput(
          "Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    levelParameter = parameters.getRawParameterValue(levelParameterId);
}

juce::AudioProcessorValueTreeState::ParameterLayout
WhiteNoiseAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ levelParameterId, 1 },
        "Level",
        juce::NormalisableRange<float>{ 0.0f, 1.0f },
        0.2f));
    return layout;
}

void WhiteNoiseAudioProcessor::prepareToPlay(double sampleRate, int)
{
    smoothedLevel.reset(sampleRate, 0.02);
    const auto initialLevel = levelParameter->load(std::memory_order_relaxed);
    smoothedLevel.setCurrentAndTargetValue(initialLevel);
}

void WhiteNoiseAudioProcessor::releaseResources()
{
}

bool WhiteNoiseAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    return layouts.inputBuses.isEmpty()
        && (output == juce::AudioChannelSet::mono()
            || output == juce::AudioChannelSet::stereo());
}

void WhiteNoiseAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const auto channelCount = getTotalNumOutputChannels();
    const auto sampleCount = buffer.getNumSamples();
    float* channels[2] = { nullptr, nullptr };

    for (int channel = 0; channel < channelCount; ++channel)
        channels[channel] = buffer.getWritePointer(channel);

    smoothedLevel.setTargetValue(levelParameter->load(std::memory_order_relaxed));

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        const auto gain = smoothedLevel.getNextValue();

        for (int channel = 0; channel < channelCount; ++channel)
        {
            const auto whiteNoise = random.nextFloat() * 2.0f - 1.0f;
            channels[channel][sample] = whiteNoise * gain;
        }
    }
}

juce::AudioProcessorEditor* WhiteNoiseAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

void WhiteNoiseAudioProcessor::getStateInformation(juce::MemoryBlock& destination)
{
    if (const auto stateXml = parameters.copyState().createXml())
        copyXmlToBinary(*stateXml, destination);
}

void WhiteNoiseAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (const auto stateXml = getXmlFromBinary(data, sizeInBytes))
    {
        if (stateXml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*stateXml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WhiteNoiseAudioProcessor();
}
