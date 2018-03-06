#include "CM_spec_web.h"
#include <QTextCodec>

extern "C" __declspec(dllexport) IsModule * loadModule()
{
	IsModule *module = new CM_spec_Module();
	return (module);
}

AFsettings *formF3settings;
CM_spec_widget::CM_spec_widget(QWidget * parent):Am_spec_widget(parent)
{
	//Создаем базовый интерфейс:
	//Добавляем пустые вкладки сдля спецификаций на окно
	widgetName = QStringLiteral("Спецификации");

	formF1 = addTab(QStringLiteral("Форма F1"));
	formF2 = addTab(QStringLiteral("Форма F2"));

	addSpace();

	formF3 = addTab(QStringLiteral("Форма F3"));
	formF3settings = new AFsettings(this);	
	formF3->ui.propsLayout->addWidget(formF3settings);
	connect(formF3settings->ui.comboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, [&]() {generateFormF3(); });

	formF4 = addTab(QStringLiteral("Форма F4"));

	addSpace();

	formF6 = addTab(QStringLiteral("Форма F6"));

}

void CM_spec_widget::IS_WidgetActived()
{
	generateFormF1();
	generateFormF2();

	generateFormF3();
	generateFormF4();

	generateFormF6();
}

void CM_spec_widget::generateFormF1()
{
	//Получаем указатель на концептуальную модель проекта
	ConceptModel *cm = (ConceptModel *)IS::getConceptModel();
	
	vector<SubjectCategory *> elements;

	//переписываем все ПК из модели в вектор
	for (int i = 0; i < cm->getConceptStructure()->elementsCount(); ++i)
	{
		elements.push_back((SubjectCategory *)cm->getConceptStructure()->getAIElement(i));
	}

	//сортируем вектор ПК по возрастанию кода, условие сортировки описано в ф-ции SubjectCategory::SCcompare
	sort(elements.begin(), elements.end(), &SubjectCategory::SCcompare);

	//Создаем 	QStandardItemModel для будущей спецификации
	QStandardItemModel form;
	form.setColumnCount(6);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("Класс"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("Код"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("Тип"));
	form.setHeaderData(4, Qt::Horizontal, QStringLiteral("Статус"));
	form.setHeaderData(5, Qt::Horizontal, QStringLiteral("Оценка"));
	
	//Проходя вектор с ПК формируем всю спецификацию
	for (auto i = elements.begin(); i < elements.end(); ++i)
	{
		//формируем строку спецификации
		//Берем текущую ПК
		SubjectCategory *sc = (*i);

		//Создаем массив для ячеек строки
		QStandardItem *row[6];

		//далее заполняем каждую ячейку строки, извлекая данные из ПК
		//и переводя их на русский язык
		QString type, status;
		switch (sc->getType())
		{
		case CT_T: type = "T"; break;
		case CT_NT: type = "NT"; break;
		}
		switch (sc->getStatus())
		{
		case CategoryStatus::CS_P: status = "P"; break;
		case CategoryStatus::CS_NP: status = "NP"; break;
		}

		//заносим данные в ячейки строки
		row[0] = new QStandardItem(QString(sc->getClassChar()));
		row[1] = new QStandardItem(QString::fromStdWString(sc->getCode().data()));
		row[2] = new QStandardItem(QString::fromStdWString(sc->getName().data()));
		row[2]->setData("text-align: left;", Qt::UserRole + 1);
		row[3] = new QStandardItem(type);
		row[4] = new QStandardItem(status);

		form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2], row[3], row[4] }));
		
	}

	//записываем полученную спецификацию на вкладку.
	fillForm(formF1, form);
}

std::wstring __sortStr = L"S0 SP PS P0 PZ ZP Z0 ZK KZ K0 KO OK O0 OR RO R0 RC CR";

void CM_spec_widget::generateFormF2()
{
	ConceptModel *cm = (ConceptModel *)IS::getConceptModel();

	QStandardItemModel form;
	form.setColumnCount(8);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("Класс"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("Код ПК1"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("Код ПК2"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("Код ПК3"));
	form.setHeaderData(4, Qt::Horizontal, QStringLiteral("Наименование"));
	form.setHeaderData(5, Qt::Horizontal, QStringLiteral("Вид"));
	form.setHeaderData(6, Qt::Horizontal, QStringLiteral("Оценка"));
	form.setHeaderData(7, Qt::Horizontal, QStringLiteral("Код"));
	vector<SCBinaryLink *> elements;

	for (int i = 0; i < cm->getConceptStructure()->linksCount(); ++i)
	{
		elements.push_back((SCBinaryLink *)cm->getConceptStructure()->getAIEBinaryLink(i));
	}

	sort(elements.begin(), elements.end(), [](SCBinaryLink *bl1, SCBinaryLink *bl2)
	{
		std::wstring s1 = bl1->getClassStr().data();
		std::wstring s2 = bl2->getClassStr().data();

		auto p1 = __sortStr.find(s1);
		auto p2 = __sortStr.find(s2);
		if (p1 < p2)
			return true;
		if (p1 > p2)
			return false;

		if (bl1->getType() < bl2->getType())
			return true;
		if (bl1->getType() > bl2->getType())
			return false;

		if (bl1->getAIE1() == bl2->getAIE1())
		{
			if (bl1->getAIE2() == bl2->getAIE2())
			{
				if (!bl1->getAIE3())
					return true;
				if (!bl2->getAIE3())
					return false;
				return SubjectCategory::SCcompare((SubjectCategory*)bl1->getAIE3(), (SubjectCategory*)bl2->getAIE3());


			}
			else
				return SubjectCategory::SCcompare((SubjectCategory*)bl1->getAIE2(), (SubjectCategory*)bl2->getAIE2());
		}
		else
			return SubjectCategory::SCcompare((SubjectCategory*)bl1->getAIE1(), (SubjectCategory*)bl2->getAIE1());

	});

	for (auto i = elements.begin(); i < elements.end(); ++i)
	{
		SCBinaryLink *sc = (*i);

		

		QString _class = QString::fromStdWString(sc->getClassStr().data());


		QString code1 = "-";
		if (sc->getAIE1())
			code1 = QString::fromStdWString(sc->getAIE1()->getCode().data());
		QString code2 = "-";
		if (sc->getAIE2())
			code2 = QString::fromStdWString(sc->getAIE2()->getCode().data());
		QString code3 = "-";
		if (sc->getAIE3())
			code3 = QString::fromStdWString(sc->getAIE3()->getCode().data());

		QString type;
		switch (sc->getType())
		{
		case LT_Arrangement: type = QStringLiteral("К"); break;
		case LT_Composition: type = QStringLiteral("С"); break;
		case LT_Order: type = QStringLiteral("У"); break;
		default:
			break;
		}
		QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
		QTextDecoder *decoder = codec->makeDecoder();

		QString name;// = decoder->toUnicode(sc->getName().data());
		name = QString::fromStdWString((sc->getName().data()));
		delete decoder;
		QString range = QString::number(sc->getRange());
		QString code = QString::fromStdWString(sc->getCode().data());

		
		QStandardItem *row[8];

		row[0] = new QStandardItem(_class);
		row[1] = new QStandardItem(code1);
		row[2] = new QStandardItem(code2);
		row[3] = new QStandardItem(code3);
		row[4] = new QStandardItem(name);
		row[4]->setData("text-align: left;", Qt::UserRole + 1);
		row[5] = new QStandardItem(type);
		row[6] = new QStandardItem(range);
		row[7] = new QStandardItem(code);

		form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7] }));
		
	}

	fillForm(formF2, form);
}

void CM_spec_widget::generateFormF3()
{
	ConceptModel *cm = (ConceptModel *)IS::getConceptModel();
	vector<AFElement *> elements;

	for (int i = 0; i < cm->getDynamicStructure(0)->getElementsCount(); ++i)
	{
		elements.push_back(cm->getDynamicStructure(0)->getAFElement(i));
	}
	if (formF3settings->ui.comboBox->currentIndex() == 0)
		sort(elements.begin(), elements.end(), AFElement::AFEcodeOrder);
	else
		sort(elements.begin(), elements.end(), AFElement::AFEcompOrder);

	
	
	QStandardItemModel form;
	form.setColumnCount(6);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("Код"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("Тип"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("Наименование"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("Статус"));
	form.setHeaderData(4, Qt::Horizontal, QStringLiteral("Степень формализации"));
	form.setHeaderData(5, Qt::Horizontal, QStringLiteral("Структурное свойство"));
	

	for (int i = 0; i < elements.size(); ++i)
	{
		
		QStandardItem *row[7];

		row[0] = new QStandardItem(QString::fromStdWString(elements[i]->getCode().data()));
		int type = elements[i]->getTypeU();
		if (type == 0)
			row[1] = new QStandardItem("-");
		else
			row[1] = new QStandardItem(QString::number(type));
		row[2] = new QStandardItem(QString::fromStdWString(elements[i]->getName().data()));
		row[2]->setData("text-align: left;", Qt::UserRole + 1);

		QString status;
		switch (elements[i]->getStatus())                                             //статус ПД
		{
		case FEStatus::Elementary: status = QStringLiteral("Э"); break;
		case FEStatus::Intermediate: status = QStringLiteral("П"); break;
		case FEStatus::Quazielementary: status = QStringLiteral("КЭ"); break;
		default:
			status = "hz =)";
			break;
		}
		QString _form;
		switch (elements[i]->getFormalization())                                //степень формализации
		{
		case FEFormalization::Algorithmic: _form = QStringLiteral("Ал."); break;
		case FEFormalization::Analytical: _form = QStringLiteral("Ан."); break;
		case FEFormalization::Empirical: _form = QStringLiteral("Э"); break;
		case FEFormalization::Information: _form = QStringLiteral("И"); break;
		case FEFormalization::Statistical: _form = QStringLiteral("С"); break;

		default:
			_form = "hz =)";
			break;
		}

		row[3] = new QStandardItem(status);
		row[4] = new QStandardItem(_form);
		row[5] = new QStandardItem(QString::fromStdWString(elements[i]->getStructureSymbols().data()));
		
		form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4], row[5] }));
	}
	
	fillForm(formF3, form);
}

void CM_spec_widget::generateFormF4()
{
	ConceptModel *cm = (ConceptModel *)IS::getConceptModel();
	vector<AFEBinaryLink *> links;
	for (int i = 0; i < cm->getDynamicStructure(0)->getLinksCount(); i++)
	{
		links.push_back(cm->getDynamicStructure(0)->getLink(i));
	}
	sort(links.begin(), links.end(), AFEBinaryLink::sortByType);

		
	QStandardItemModel form;
	form.setColumnCount(6);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("Код ПЗ-1"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("Код ПЗ-2"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("Код ПЗ-3"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("Оценка"));
	form.setHeaderData(4, Qt::Horizontal, QStringLiteral("Вид"));
	form.setHeaderData(5, Qt::Horizontal, QStringLiteral("Код"));


	for (int i = 0; i < links.size(); ++i)
	{

		QStandardItem *row[6];
		
		AFEBinaryLink  *l = links[i];

		QString code1, code2, code3;

		
		code1 = QString::fromStdWString(l->getAFE1()->getCode().data());
		code2 = QString::fromStdWString(l->getAIE2()->getCode().data());
		if (l->getAIE3() && l->getAFE1()->getType() == FEType::Sequanse)
			code3 = QString::fromStdWString(l->getAIE3()->getCode().data());
		else
		{

			code3 = "-";
		}

		row[0] = new QStandardItem(code1);
		row[1] = new QStandardItem(code2);
		row[2] = new QStandardItem(code3);

		QString linkType;

		if (links[i]->getType() == LT_Arrangement)
		{

			if (l->getAFE1()->getType() == FEType::Sequanse && code3 == "-")
				continue;

			linkType = QStringLiteral("К");
			QString type;
			switch (l->getAFE1()->getType())            //в зависимости от типа ПД1  пишем последний столбик в форме А 
			{
			case FEType::Sequanse: type = QStringLiteral("Последовательность"); break;
			case FEType::Iteration: type = QStringLiteral("Цикл"); break;
			case FEType::Alternate: type = QStringLiteral("Альтернатива"); break;
			default:
				type = "hz =)";
				break;
			}
			row[3] = new QStandardItem(type);

		}

		if (links[i]->getType() == LT_Composition)
		{
			linkType = QStringLiteral("C");
			row[3] = new QStandardItem("1");
		}


		row[4] = new QStandardItem(linkType);
		row[5] = new QStandardItem(code1 + code2 + code3);
		
		form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5] }));
	}
	fillForm(formF4, form);
	
}

void CM_spec_widget::generateFormF6()
{
	ConceptModel *am = IS::getConceptModel();
	vector<AFElement *> elements;

	for (int i = 0; i < am->getDynamicStructure(0)->getElementsCount(); ++i)
	{
		elements.push_back(am->getDynamicStructure(0)->getAFElement(i));
	}
	sort(elements.begin(), elements.end(), AFElement::AFEcompOrder);

	QStandardItemModel form;
	form.setColumnCount(7);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("Код ПЗ-1"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("Структ. св-во ПЗ-1"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("Код ПК"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("Роль ПК"));
	form.setHeaderData(4, Qt::Horizontal, QStringLiteral("Структ. св-во ПК"));
	form.setHeaderData(5, Qt::Horizontal, QStringLiteral("Объем. св-во ПК"));
	form.setHeaderData(6, Qt::Horizontal, QStringLiteral("Особ. роль ПК"));

	for (int i = 0; i < elements.size(); ++i)
	{
		QString pzCode = QString::fromStdWString(elements[i]->getCode().data());
		QString strSymbol = QString::fromStdWString(elements[i]->getStructureSymbols().data());
		bool functionFound = false;

		foreach(QChar c, strSymbol)
		{


			QString strPropPK;
			QString pkCode;
			if (c == '[' || c == ']')
			{
				strPropPK = QStringLiteral("ФП");
			}
			if (c == '{' || c == '}')
			{
				strPropPK = QStringLiteral("ФЦ");
			}
			if (c == '$')
			{
				strPropPK = QStringLiteral("ФИ");
			}
			if (c == '/' || c == '?')
			{
				strPropPK = QStringLiteral("АА");
			}
			if (c == '!')
			{
				strPropPK = QStringLiteral("СЦ");
			}
			if (c == '#')
			{
				strPropPK = QStringLiteral("ФА");
			}


			if (c == ']' || c == '}' || c == '[' || c == '{' || c == '#')
			{
				AIElement *e = elements[i]->getFunction();
				QString f = "";
				pkCode = "";
				if (e)
				{
					pkCode = QString::fromStdWString(e->getCode().data());
					if (elements[i]->getType() == None)
					{
						f = QStringLiteral("Ф");
						functionFound = true;
					}
				}
				
				QStandardItem *row[7];
				row[0] = new QStandardItem(pzCode);
				row[1] = new QStandardItem(c);
				row[2] = new QStandardItem(pkCode);
				row[3] = new QStandardItem(f);
				row[4] = new QStandardItem();
				row[5] = new QStandardItem();
				row[6] = new QStandardItem(strPropPK);
				form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6]}));
				
			}
			if (c == '/' || c == '?' || c=='!')
			{
				ALElementList _list = elements[i]->getContentArgs(c.toLatin1());
				for (int i = 0; i < _list.count(); ++i)
				{
					AIElement *e = _list.element<AIElement*>(i);
					pkCode = QString::fromStdWString(e->getCode().data());
										
					QStandardItem *row[7];
					row[0] = new QStandardItem(pzCode);
					row[1] = new QStandardItem(c);
					row[2] = new QStandardItem(pkCode);
					row[3] = new QStandardItem();
					row[4] = new QStandardItem();
					row[5] = new QStandardItem();
					row[6] = new QStandardItem(strPropPK);
					form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6] }));
				}
			}
			if (c == '$')
			{
				AIElement *func = elements[i]->getAutoArg(c.toLatin1());
				if (func)
				{
					QStandardItem *row[7];
					row[0] = new QStandardItem(pzCode);
					row[1] = new QStandardItem(c);
					row[2] = new QStandardItem(QString::fromStdWString(func->getCode().data()));
					row[3] = new QStandardItem();
					row[4] = new QStandardItem();	row[5] = new QStandardItem();
					row[6] = new QStandardItem(strPropPK);
					form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6] }));
				}

			}

		}  //foreach(QChar c, strSymbol)

		if (!functionFound && elements[i]->getType() == None)
		{
			AIElement *e = elements[i]->getFunction();
			if (e)
			{				
				QStandardItem *row[7];
				row[0] = new QStandardItem(pzCode);
				row[1] = new QStandardItem();
				row[2] = new QStandardItem(QString::fromStdWString(e->getCode().data()));
				row[3] = new QStandardItem(QStringLiteral("Ф"));
				row[4] = new QStandardItem();
				row[5] = new QStandardItem();
				row[6] = new QStandardItem();
				form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6] }));
			}
		}
		{
			ALElementList _list = elements[i]->getContentArgs('+');
			for (int i = 0; i < _list.count(); ++i)
			{
				AIElement *e = _list.element<AIElement*>(i);
				QString pkCode = QString::fromStdWString(e->getCode().data());
				
				QStandardItem *row[7];
				row[0] = new QStandardItem(pzCode);
				row[1] = new QStandardItem();
				row[2] = new QStandardItem(QString::fromStdWString(e->getCode().data()));
				row[3] = new QStandardItem(QStringLiteral("А"));
				row[4] = new QStandardItem();row[5] = new QStandardItem();row[6] = new QStandardItem();
				form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6] }));
			}
		}
		{
			ALElementList _list = elements[i]->getContentArgs('.');
			for (int i = 0; i < _list.count(); ++i)
			{
				AIElement *e = _list.element<AIElement*>(i);
				QString pkCode = QString::fromStdWString(e->getCode().data());
								
				QStandardItem *row[7];
				row[0] = new QStandardItem(pzCode);
				row[1] = new QStandardItem();
				row[2] = new QStandardItem(QString::fromStdWString(e->getCode().data()));
				row[3] = new QStandardItem(QStringLiteral("А-у"));
				row[4] = new QStandardItem(); row[5] = new QStandardItem(); row[6] = new QStandardItem();
				form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6] }));
			}
		}
		{
			ALElementList _list = elements[i]->getAutoArgs('.');
			for (int i = 0; i < _list.count(); ++i)
			{
				AIElement *e = _list.element<AIElement*>(i);
				QString pkCode = QString::fromStdWString(e->getCode().data());
				QStandardItem *row[7];
				row[0] = new QStandardItem(pzCode);
				row[1] = new QStandardItem();
				row[2] = new QStandardItem(QString::fromStdWString(e->getCode().data()));
				row[3] = new QStandardItem(QStringLiteral("А-у"));
				row[4] = new QStandardItem(); row[5] = new QStandardItem(); row[6] = new QStandardItem();
				form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3],row[4],row[5],row[6] }));
			}
		}

	}
	fillForm(formF6, form);

}
