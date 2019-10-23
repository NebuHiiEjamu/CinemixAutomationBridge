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

#pragma once

#include <memory>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class AutomationBridgeSettings;
class PluginMainPanel;

/**
*/
class AutomationBridgeEditor  : public AudioProcessorEditor
{
public:
    AutomationBridgeEditor (AutomationBridge&);
    ~AutomationBridgeEditor();

    void paint (Graphics&) override;
    void resized() override;

public:
    AutomationBridge& processor;
	std::unique_ptr<AutomationBridgeSettings> prefsPanel;
    std::unique_ptr<PluginMainPanel> mainPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AutomationBridgeEditor)
};

