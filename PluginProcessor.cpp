/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A2StarterAudioProcessor::A2StarterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

A2StarterAudioProcessor::~A2StarterAudioProcessor()
{
}

//==============================================================================
const juce::String A2StarterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool A2StarterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool A2StarterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool A2StarterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double A2StarterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int A2StarterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int A2StarterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void A2StarterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String A2StarterAudioProcessor::getProgramName (int index)
{
    return {};
}

void A2StarterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void A2StarterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const int numberOfInputChannels = getTotalNumInputChannels();
    
    rate = static_cast<float> (sampleRate);
    
    wet = 50.0;
    dry = 50.0;
    delayTime = 3.0;
    feedback = 0.0;
    
    positionInBuffer = 0;
    
    // Need to change this value to a number that corresponds to 3 seconds
    //delayBufferLength = 1;
   // delayBufferLength = (int)(3 * (sampleRate + samplesPerBlock));
    delayBufferLength = (int)(3 * sampleRate);

    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();

}

void A2StarterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool A2StarterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void A2StarterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    int numSamples = buffer.getNumSamples();
    int delayNumSamples = delayBuffer.getNumSamples();
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* data = buffer.getWritePointer (channel,0);
        const float *bufferDataRead = buffer.getReadPointer(channel);
        const float *delayBufferDataRead = delayBuffer.getReadPointer(channel);
        
        //Adjust volume of the notes based on the dry value
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
          {
              data[i] *= dry*0.01; // Multiply each sample by dryness percentage
          }
    
        
        // Get a pointer to write in the delayBuffer
        float* delayData = delayBuffer.getWritePointer(channel);
        
        /*we want to keep track of where we are in the delay buffer and how much left there is to fill in the delay buffer
         */
        if (delayNumSamples > numSamples + positionInBuffer){
            delayBuffer.copyFromWithRamp(channel, positionInBuffer, bufferDataRead, numSamples, wet*0.01, wet*0.01);
        }
        else {
            int amountLeftInBuffer = delayBufferLength - positionInBuffer;
            delayBuffer.copyFromWithRamp(channel, positionInBuffer, bufferDataRead, amountLeftInBuffer, wet*0.01, wet*0.01);
            delayBuffer.copyFromWithRamp(channel, 0, bufferDataRead, numSamples - amountLeftInBuffer, wet*0.01, wet*0.01);
        }
        
        //now we have our delay buffer copied, we want to figure out how
        //to get the proper value from the delay buffer based on time delay
        int readPosition = (int) (delayBufferLength + positionInBuffer - (rate * delayTime)) % delayBufferLength;
        
        if (delayNumSamples > numSamples + readPosition) {
            buffer.addFrom(channel, 0, delayBufferDataRead + readPosition, numSamples);
        }
        else{
            int bufferLeft = delayNumSamples - readPosition;
            buffer.addFrom(channel, 0, delayBufferDataRead + readPosition, bufferLeft);
            buffer.addFrom(channel, bufferLeft, delayBufferDataRead, numSamples - bufferLeft);
        }
        
        if (feedback > 0.0){
            
            if (delayNumSamples > numSamples + positionInBuffer) {
                delayBuffer.addFromWithRamp(channel, positionInBuffer, data, numSamples, wet*0.01/feedback, wet*0.01/feedback);
                
            }
            else {
                int leftInBuffer = delayNumSamples - positionInBuffer;
                delayBuffer.addFromWithRamp(channel, leftInBuffer, data, leftInBuffer, wet*0.01/feedback, wet*0.01/feedback);
                delayBuffer.addFromWithRamp(channel, 0, data, numSamples - leftInBuffer, wet*0.01/feedback, wet*0.01/feedback);
            }
             
        }
        
        //////TESTING AREA

        
        // Mix a portion of the output back into the delay buffer with feedback
        /*for (int i = 0; i < numSamples; ++i) {
            delayData[positionInBuffer] += data[i] * feedback*0.01;
            // Ensure the delay buffer doesn't clip (optional)
            if (delayData[positionInBuffer] > 1.0f)
                delayData[positionInBuffer] = 1.0f;
            if (delayData[positionInBuffer] < -1.0f)
                delayData[positionInBuffer] = -1.0f;
            positionInBuffer = (positionInBuffer + 1) % delayNumSamples;
        }*/
        
    }
    
    positionInBuffer = (numSamples + positionInBuffer) % delayNumSamples;

}

//==============================================================================
bool A2StarterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* A2StarterAudioProcessor::createEditor()
{
    return new A2StarterAudioProcessorEditor (*this);
}

//==============================================================================
void A2StarterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void A2StarterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new A2StarterAudioProcessor();
}
