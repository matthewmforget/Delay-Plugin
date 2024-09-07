/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A2StarterAudioProcessorEditor::A2StarterAudioProcessorEditor (A2StarterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 700);
    
    
    // these define the parameters of our slider object
    feedbackSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    feedbackSlider.setRange (0.0, 100.0, 1.00);
    feedbackSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    feedbackSlider.setPopupDisplayEnabled (true, false, this);
    feedbackSlider.setTextValueSuffix (" Feedback");
    feedbackSlider.setValue(0.0);
    
    myButton.setButtonText("Ping-Pong");
    myButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
    myButton.setClickingTogglesState(true);
    //myButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::grey);
    
    timeIntervalSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    timeIntervalSlider.setRange (0.0, 3.0, 0.1);
    timeIntervalSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    timeIntervalSlider.setPopupDisplayEnabled (true, false, this);
    timeIntervalSlider.setTextValueSuffix (" Delay Time");
    timeIntervalSlider.setValue(3.0);
    
    drySlider.setSliderStyle (juce::Slider::LinearBarVertical);
    drySlider.setRange (0.0, 100.0, 1.0);
    drySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    drySlider.setPopupDisplayEnabled (true, false, this);
    drySlider.setTextValueSuffix (" Dryness");
    drySlider.setValue(50.0);
    
    wetSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    wetSlider.setRange (0.0, 100.0, 1.0);
    wetSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    wetSlider.setPopupDisplayEnabled (true, false, this);
    wetSlider.setTextValueSuffix (" Wetness");
    wetSlider.setValue(50.0);
 
    // this function adds the slider to the editor
    addAndMakeVisible (&feedbackSlider);
    feedbackSlider.addListener (this);
    
    addAndMakeVisible (&timeIntervalSlider);
    timeIntervalSlider.addListener (this);
    
    addAndMakeVisible (&drySlider);
    drySlider.addListener (this);
    
    addAndMakeVisible (&wetSlider);
    wetSlider.addListener (this);
    
    addAndMakeVisible(myButton);
}

A2StarterAudioProcessorEditor::~A2StarterAudioProcessorEditor()
{
}

//==============================================================================
void A2StarterAudioProcessorEditor::paint (juce::Graphics& g)
{
   // fill the whole window white
    g.fillAll (juce::Colours::white);
 
    // set the current drawing colour to black
    g.setColour (juce::Colours::black);
 
    // set the font size and draw text to the screen
    g.setFont (15.0f);
 
    g.drawFittedText ("A2 Delay", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void A2StarterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
    feedbackSlider.setBounds (40, 30, 20, getHeight() - 40);
    timeIntervalSlider.setBounds(feedbackSlider.getX() + 80, feedbackSlider.getY(), feedbackSlider.getWidth(), feedbackSlider.getHeight());
    drySlider.setBounds(feedbackSlider.getX() + 160, feedbackSlider.getY(), feedbackSlider.getWidth(), feedbackSlider.getHeight());
    wetSlider.setBounds(feedbackSlider.getX() + 240, feedbackSlider.getY(), feedbackSlider.getWidth(), feedbackSlider.getHeight());
    myButton.setBounds(wetSlider.getX() +20, getHeight() - 500, 100, 70);
}

void A2StarterAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.feedback = feedbackSlider.getValue();
    audioProcessor.delayTime = timeIntervalSlider.getValue();
    audioProcessor.dry = drySlider.getValue();
    audioProcessor.wet = wetSlider.getValue();
}
