/*
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation files
 (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 Any person wishing to distribute modifications to the Software is
 asked to send the modifications to the original developer so that
 they can be incorporated into the canonical version.  This is,
 however, not a binding provision of this license.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginMainPanel.h"
#include "PluginEditor.h"
#include "PluginSettings.h"

//==============================================================================
PluginMainPanel::PluginMainPanel (AutomationBridgeEditor& e)
: editor (e)
{
    
    addAndMakeVisible (prefsButton);
    prefsButton.setButtonText ("Settings");
    prefsButton.changeWidthToFitText();
    prefsButton.onClick = [this, &e] {
        e.getPrefsPanel()->setVisible (true);
        setVisible (false);
    };
    
    addAndMakeVisible (testModeToggle);
    testModeToggle.setButtonText ("Test Mode");
    testModeToggle.setClickingTogglesState (true);
    testModeToggle.changeWidthToFitText();
    testModeToggle.onClick = [this, &e] {
        //e.getProcessor().testMode();
    };
    
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
    Rectangle<int> faderLayout = area.removeFromLeft (area.getWidth() * 0.75f);
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
