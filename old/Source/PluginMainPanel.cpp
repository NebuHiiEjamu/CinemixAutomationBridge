/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginMainPanel.h"
#include "PluginEditor.h"
#include "PluginSettings.h"
//#include "TouchPad.h"

//==============================================================================
PluginMainPanel::PluginMainPanel (AutomationBridgeEditor& e)
: editor (e)
{
    addAndMakeVisible (prefsButton);
    prefsButton.setButtonText ("Settings");
    prefsButton.changeWidthToFitText();
    prefsButton.onClick = [this, &e] {
        e.prefsPanel->setVisible (true);
        setVisible (false);
    };
    
    addAndMakeVisible (testModeToggle);
    testModeToggle.setButtonText ("Test Mode");
    testModeToggle.setClickingTogglesState (true);
    testModeToggle.changeWidthToFitText();
    testModeToggle.onClick = [this, &e] {
        e.processor.testMode();
    };

    for (int i = 0; i < editor.processor.faders; i++)
    {
        Slider* newFader = new Slider();
        addAndMakeVisible (dynamic_cast<Component*> (newFader));
        newFader->setSliderStyle (Slider::LinearVertical);
        newFader->setRange (0.0, 127.0, 1.0);
        faders.add (newFader);
        
        TextButton* newMute = new TextButton("M");
        addAndMakeVisible (dynamic_cast<Component*> (newMute));
        newMute->changeWidthToFitText();
        newMute->setClickingTogglesState (true);
        newMute->onClick = [this, &e] {
        };
        mutes.add (newMute);
        
        Label* newLabel = new Label();
        addAndMakeVisible (dynamic_cast<Component*> (newLabel));
        newLabel->setText (String (i + 1), NotificationType::dontSendNotification);
        faderIds.add (newLabel);
    }
    
    setOpaque (true);
    setVisible (true);
}

PluginMainPanel::~PluginMainPanel()
{
}

void PluginMainPanel::paint (Graphics& g)
{
    g.fillAll (Colours::blue);
    /*g.drawImage (background, 0, 0, background.getWidth(), background.getHeight(), 0, 0,
     background.getWidth(), background.getHeight());*/
}

void PluginMainPanel::resized()
{
    Rectangle<int> area = getLocalBounds();
    Rectangle<int> header = area.removeFromTop (25);
    Rectangle<int> faderLayout = area.removeFromLeft (roundToInt (area.getWidth() * 0.75f));
    prefsButton.setBounds (header.removeFromLeft (100));
    testModeToggle.setBounds (header.removeFromRight (100));
    
    Rectangle<int> faderPair;
    for (int i = 0; i < faders.size(); i++)
    {
        Rectangle<int> faderMute;
        
        if (i % 2 == 0)
        {
            faderPair = faderLayout.removeFromLeft (25);
            faderMute = faderPair.removeFromTop (faderPair.getHeight() / 2);
        }
        else
            faderMute = faderPair;
            faders[i]->setBounds (faderPair);
        
        faderIds[i]->setBounds (faderMute.removeFromBottom (25));
        mutes[i]->setBounds (faderMute.removeFromBottom (25));
        faders[i]->setBounds (faderMute);
    }
}
