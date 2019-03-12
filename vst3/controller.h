#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"

class PlugController : public Vst::EditController, public VSTGUI::VST3EditorDelegate
{
public:
	static FUnknown* createInstance(void*)
	{
		return dynamic_cast<Vst::IEditController *>(new PlugController());
	}

	tresult PLUGIN_API initialize(FUnknown *context) override;
	IPlugView* PLUGIN_API createView(const char *name) override;
	tresult PLUGIN_API setComponentState(IBStream *state) override;
};
