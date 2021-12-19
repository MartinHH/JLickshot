/*
 * Copyright 2014 Martin Hansen
 *
 * This file is part of JLickshot.
 *
 * JLickshot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JLickshot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JLickshot.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JuceHeader.h"
#include "SampleChoiceComponent.h"

SampleChoiceComponent::SampleChoiceComponent(int noteNumber):
    noteNo_(noteNumber)
{
    noteLabel_ = new Label ("note label",
                            MidiMessage::getMidiNoteName(noteNo_, true, true, 3) + ":");
    addAndMakeVisible (noteLabel_);
    noteLabel_->setFont (Font (15.00f, Font::plain));
    noteLabel_->setJustificationType (Justification::centredLeft);
    noteLabel_->setEditable (false, false, false);
    noteLabel_->setColour (Label::textColourId, Colours::black);
    noteLabel_->setColour (Label::backgroundColourId, Colour (0x00000000));
    noteLabel_->setTooltip(translate("MIDI note number ") + String(noteNumber));
    
    addAndMakeVisible (nameDisplay_ = new TextEditor ("name display"));
    nameDisplay_->setMultiLine (false);
    nameDisplay_->setReturnKeyStartsNewLine (false);
    nameDisplay_->setReadOnly (true);
    nameDisplay_->setScrollbarsShown (true);
    nameDisplay_->setCaretVisible (false);
    nameDisplay_->setPopupMenuEnabled (true);
    nameDisplay_->setTooltip(translate("You can drag audio files here"));
    nameDisplay_->setText (translate("empty"));
    
    addAndMakeVisible (fileButton_ = new TextButton ("file button"));
    fileButton_->setButtonText (translate("Select File"));
    fileButton_->setTooltip(translate("Select an audio file for this slot"));
    fileButton_->addListener (this);
    
    addAndMakeVisible (velocitySlider_ = new Slider ("velocity slider"));
    velocitySlider_->setRange (0, 1.0, 0.01);
    velocitySlider_->setValue(0.75);
    velocitySlider_->setSliderStyle (Slider::LinearBar);
    velocitySlider_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    velocitySlider_->setTooltip(translate("Individual gain for this slot"));
    velocitySlider_->addListener (this);
    
    setSize (500, 24);
}

SampleChoiceComponent::~SampleChoiceComponent()
{
    noteLabel_ = nullptr;
    nameDisplay_ = nullptr;
    fileButton_ = nullptr;
    velocitySlider_ = nullptr;
}

void SampleChoiceComponent::addListener(SampleChoiceComponent::Listener *listener)
{
    listeners_.add(listener);
}

void SampleChoiceComponent::removeListener(SampleChoiceComponent::Listener *listener)
{
    listeners_.remove(listener);
}

void SampleChoiceComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background
}

void SampleChoiceComponent::resized()
{
    // from left:
    noteLabel_->setBounds (0, 0, 40, getHeight());

    // from right:
    velocitySlider_->setBounds (getWidth() - 36, 0, 30, getHeight());
    fileButton_->setBounds (velocitySlider_->getX() - 88, 0, 78, getHeight());
    
    // takes up space in between:
    const int nameX = noteLabel_->getWidth() + 8;
    const int nameW = fileButton_->getX() - nameX - 8;
    nameDisplay_->setBounds (nameX, 0, nameW, getHeight());
}

void SampleChoiceComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == velocitySlider_){
        Component::BailOutChecker checker(this);
        listeners_.callChecked(checker,
                               &SampleChoiceComponent::Listener::velocityChanged,
                               this);
    }
}

void SampleChoiceComponent::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == fileButton_){
        fileCooser_ = std::make_unique<FileChooser>("Choose a file to open...",
                                                    File::getCurrentWorkingDirectory(),
                                                    "*.mp3; *.wav; *.aif",
                                                    true);
        
        auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
     
        fileCooser_->launchAsync(folderChooserFlags, [this] (const FileChooser& chooser)
        {
            File file (chooser.getResult());
            
            if(!file.getFullPathName().isEmpty()) {
                file.getParentDirectory().setAsCurrentWorkingDirectory();
                
                Component::BailOutChecker checker(this);
                listeners_.callChecked(checker,
                                       &SampleChoiceComponent::Listener::fileChosen,
                                       this, file);
            }
     
        }
        );
    }
}

int SampleChoiceComponent::getNoteNumber() const
{
    return noteNo_;
}

void SampleChoiceComponent::setFileName(const juce::String &name)
{
    nameDisplay_->setText(name);
    repaint();
}

float SampleChoiceComponent::getVelocity() const
{
    return (float) velocitySlider_->getValue();
}

void SampleChoiceComponent::setVelocity(float velocity)
{
    velocitySlider_->setValue(velocity);
    repaint();
}

bool SampleChoiceComponent::isInterestedInFileDrag(const StringArray &files)
{
    return files.size() == 1 && (files[0].endsWith(translate(".wav"))
                              || files[0].endsWith(translate(".aif"))
                              || files[0].endsWith(translate(".aiff"))
                              || files[0].endsWith(translate(".mp3")));
}

void SampleChoiceComponent::filesDropped(const juce::StringArray &files, int /* x */, int /* y */)
{
    const File file(files[0]);
    Component::BailOutChecker checker(this);
    listeners_.callChecked(checker,
                           &SampleChoiceComponent::Listener::fileChosen,
                           this, file);
}