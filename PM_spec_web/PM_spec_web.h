#pragma once

#include "pm_spec_web_global.h"

#include "..\AM_spec_web\AM_spec_web.h"

extern "C" __declspec(dllexport) IsModule * loadModule();

class PM_spec_widget : public Am_spec_widget
{
	Q_OBJECT
public:
	PM_spec_widget(QWidget *parent=0);

	void IS_WidgetActived();
	
	FormAndPanel *formA, *formB, *formD, *formD1, *formD2, *formD3;
	AFsettings *formBsettings;
	AFsettings *formDsettings;

	void generateFormA();
	void generateFormB();
	void generateFormD();
	void generateFormD1();
	void generateFormD2();

};



class PM_spec_Module : public IsModule
{
public:
	PM_spec_Module() {};
	~PM_spec_Module() {};
	void showModuleWidget()
	{
		PM_spec_widget *th = new PM_spec_widget();
		RegisterModuleWidget(th);
	}

	ModuleInfo* setModuleInfo()
	{
		ModuleInfo *mi = new ModuleInfo;
		mi->description = "PM_spec_web";
		mi->version = 0.1;
		mi->name = "PM_spec_web";
		return mi;
	}

};