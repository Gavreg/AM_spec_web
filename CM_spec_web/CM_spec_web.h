#pragma once

#include "cm_spec_web_global.h"

#include "..\AM_spec_web\AM_spec_web.h"

extern "C" __declspec(dllexport) IsModule * loadModule();


//окно модуля для генерации форм начальной модели 
//см описание базового класса в AM_spec_web.h
class CM_spec_widget : public Am_spec_widget
{
	Q_OBJECT
public:
	CM_spec_widget(QWidget *parent = 0);

	//функция срабатывает, когда ИС-2 переключается на модуль 
	//Именно в ней инициируется создание спецификаций
	void IS_WidgetActived();
	
	//визуальные формы. Описание см в AM_spec_web.h
	FormAndPanel *formF1, *formF2, *formF3, *formF4, *formF6;
	AFsettings *formBsettings;
	AFsettings *formDsettings;

	//Вся генерация форм лежит в этих независимых друг от друга функциях
	void generateFormF1();
	void generateFormF2();
	void generateFormF3();
	void generateFormF4();
	void generateFormF6();

};


 //Описание модуля ИС-2
class CM_spec_Module : public IsModule
{
public:

	void showModuleWidget()
	{
		CM_spec_widget *th = new CM_spec_widget();
		RegisterModuleWidget(th);
	}

	ModuleInfo* setModuleInfo()
	{
		ModuleInfo *mi = new ModuleInfo;
		mi->description = "CM_spec_web";
		mi->version = 0.1;
		mi->name = "CM_spec_web";
		return mi;
	}

};