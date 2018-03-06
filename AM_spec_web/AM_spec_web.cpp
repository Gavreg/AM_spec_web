#include "AM_spec_web.h"

#include <QtWebEngineWidgets/QWebEngineView>
#include <QFile>

AM_spec_web::AM_spec_web()
{
}


Am_spec_widget::Am_spec_widget(QWidget * parent):WorkWidget(parent)
{
	ui.setupUi(this);
}




//добавление пустой вкладки-спецификации с нужным именем  на коно виджета
FormAndPanel * Am_spec_widget::addTab(QString & name)
{
	FormAndPanel * r = new FormAndPanel();
	ui.tabWidget->addTab(r, name);
	return r;
}




//добавление пустого пространства между вкладками спецификаций (чтобы было красиво)
void Am_spec_widget::addSpace()
{
	QWidget *w = new QWidget();
	
	
	ui.tabWidget->setTabEnabled(ui.tabWidget->addTab(w, ""), false);
}

void Am_spec_widget::fillForm(FormAndPanel *form, QStandardItemModel &table)

{	 
	QWebEngineView *web = form->ui.webEngineView;
	web->setZoomFactor(ui.sbZoom->value() / 100.0);	


	QFile f(QApplication::applicationDirPath() + "\\pm_template\\formA.html");	 
	f.open(QIODevice::ReadOnly);	
	QTextStream s(&f);	  
	QString str = s.readAll();	 
	f.close();	   

	web->setHtml(str.replace("%formA%", modelToHtml(table)));

}

//меняем значение масштаба во всех вкладках в зависимости от значения i
void Am_spec_widget::on_sbZoom_valueChanged(int i)
{
	//QApplication::beep();
	foreach(QWebEngineView *web, findChildren<QWebEngineView*>())
	{
		web->setZoomFactor(i / 100.0);
	}
}

 //преобразование  QStandardItemModel в HTML таблицу <table> ... </table>
QString modelToHtml(QStandardItemModel & model)
{
	QString table="<table>";
	QString header = "<tr>";
	for (int i = 0; i < model.columnCount(); ++i)
	{
		header += "<th>" + model.headerData(i, Qt::Orientation::Horizontal).toString() + "</th>";
	}
	header += "</tr>";

	table += header;

	for (int i = 0; i < model.rowCount(); ++i)
	{
		QString row="<tr>";
		for (int j = 0; j < model.columnCount(); ++j)
		{
			QString style = model.data(model.index(i, j), Qt::UserRole + 1).toString();
			row += "<td style=\""+style+"\">" + model.data(model.index(i, j)).toString() + "</td>";
		}
		row += "</tr>";
		table += row;
	}
	table += "</table>";

	return table;
}

FormAndPanel::FormAndPanel(QWidget *parent):QWidget(parent)
{
	ui.setupUi(this);
}

AFsettings::AFsettings(QWidget * parent):QWidget(parent)
{
	ui.setupUi(this);
}
