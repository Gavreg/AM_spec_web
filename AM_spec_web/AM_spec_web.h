#pragma once

#include "am_spec_web_global.h"
#include <is>
#include <pm>
#include <cm>

#include "..\AM_spec_web\GeneratedFiles\ui_am_spec_widget.h"
#include "..\AM_spec_web\GeneratedFiles\ui_AFsettings.h"
#include "..\AM_spec_web\GeneratedFiles\ui_formAndPanel.h"

#include <QStandardItemModel>


QString modelToHtml(QStandardItemModel &model);

class FormAndPanel;
class AFsettings;


//главный виджет модуля. Содержит tabWidget c вкладками спецификаций
class Am_spec_widget : public WorkWidget
{
	Q_OBJECT

public:
	Am_spec_widget(QWidget *parent = 0);

	Ui_am_spec_widget ui;
	
	
	FormAndPanel *addTab(QString &name);
	void fillForm(FormAndPanel *, QStandardItemModel &table);
	void addSpace();

	

public slots:

	void on_sbZoom_valueChanged(int i);
};

//Виджет для вывода одной спецификации
class FormAndPanel : public QWidget
{
	Q_OBJECT
public:
	FormAndPanel(QWidget *parent = 0);
	Ui_formAndPanel ui;
};

//виджет для настройки порядка сортировки ПК
class AFsettings : public QWidget
{
	Q_OBJECT
public:
	AFsettings(QWidget *parent = 0);
	Ui_afSettings ui;
};

class AM_SPEC_WEB_EXPORT AM_spec_web
{
public:
	AM_spec_web();

	
};
