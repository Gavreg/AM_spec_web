#include "PM_spec_web.h"

extern "C" __declspec(dllexport) IsModule * loadModule()
{
	IsModule *module = new PM_spec_Module();
	return (module);
}



PM_spec_widget::PM_spec_widget(QWidget * parent):Am_spec_widget(parent)
{
	widgetName = QStringLiteral("������������");

	formA = addTab(QStringLiteral("����� �"));

	formB = addTab(QStringLiteral("����� B"));
	formBsettings = new AFsettings(this);
	formB->ui.propsLayout->addWidget(formBsettings);
	connect(formBsettings->ui.comboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, [&]() {generateFormB(); });

	addSpace();
	formD1 = addTab(QStringLiteral("����� D1"));

	addSpace();
	formD = addTab(QStringLiteral("����� D"));
	formDsettings = new AFsettings(this);
	formD->ui.propsLayout->addWidget(formDsettings);
	connect(formDsettings->ui.comboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, [&]() {generateFormD(); });

	formD2 = addTab(QStringLiteral("����� D2"));
}

void PM_spec_widget::IS_WidgetActived()
{
	//ui.tabWidget->clear();

	generateFormA();
	generateFormB();
    generateFormD();
	generateFormD1();
	generateFormD2();
}

void PM_spec_widget::generateFormA()
{
	QStandardItemModel form;
	
	vector<AFEBinaryLink *> links;
	AbstractModel *am = IS::getPrimaryModel();
	for (int i = 0; i < am->getDynamicStructure(0)->getLinksCount(); i++)
	{
		if (am->getDynamicStructure(0)->getLink(i)->getType() == LT_Arrangement)
			links.push_back(am->getDynamicStructure(0)->getLink(i));
	}
	sort(links.begin(), links.end(), AFEBinaryLink::sortByCodes);


	
	form.setColumnCount(4);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("��� ��1"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("��� ��2"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("��� ��3"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("��� ����������"));

	
	for (int i = 0; i < links.size(); ++i)
	{		
		
		AFEBinaryLink  *l = links[i];

		QVector<QStandardItem *>row;
		if (l->getAFE1()->getType() == FEType::Sequanse && l->getAIE3() == 0)
			continue;
		
		row.push_back(new QStandardItem(QString::fromWCharArray(l->getAFE1()->getCode().data())));//�������� ��� ��1
		row.push_back(new QStandardItem(QString::fromWCharArray(l->getAIE2()->getCode().data()))); //�������� ��� ��2
                   
		if (l->getAIE3() && l->getAFE1()->getType() == FEType::Sequanse)
			row.push_back(new QStandardItem(QString::fromWCharArray(l->getAIE3()->getCode().data())));                //���� � ����� ����������  ������ ��, �������� ��� ���
		else
			row.push_back(new QStandardItem("-") );                                 //� ��������� ������ ������ �����



		QString type;
		switch (l->getAFE1()->getType())            //� ����������� �� ���� ��1  ����� ��������� ������� � ����� � 
		{
		case FEType::Sequanse: type = QStringLiteral("������������������"); break;
		case FEType::Iteration: type = QStringLiteral("����"); break;
		case FEType::Alternate: type = QStringLiteral("������������"); break;
		default:
			type = "hz =)";
			break;
		}
		row.push_back(new QStandardItem(type));

		form.appendRow(row.toList());
	}

	fillForm(formA, form);
	
}

void PM_spec_widget::generateFormB()
{
	QStandardItemModel form;
	AbstractModel *am = IS::getPrimaryModel();
	

	vector<AFElement *> elements;
	for (int i = 0; i < am->getDynamicStructure(0)->getElementsCount(); ++i) //�������� ��� �� ��������� ������
	{
		elements.push_back(am->getDynamicStructure(0)->getAFElement(i));
	}
	if (formBsettings->ui.comboBox->currentIndex() == 0)	 //����� ������� ���������
		sort(elements.begin(), elements.end(), AFElement::AFEcodeOrder);  //���������� �� ���� 
	else
		sort(elements.begin(), elements.end(), AFElement::AFEcompOrder);  //���������� �� ������� ������

	form.setColumnCount(4);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("���"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("������������"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("������"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("������� ������������"));

	

	for (int i = 0; i < elements.size(); ++i)
	{
		QStandardItem *row[4];
	
		row[0] = new QStandardItem(QString::fromWCharArray(elements[i]->getCode().data()));
		row[1] = new QStandardItem(QString::fromWCharArray(elements[i]->getName().data()));

		QString status;
		switch (elements[i]->getStatus())                                             //������ ��
		{
		case FEStatus::Elementary: status = QStringLiteral("�"); break;
		case FEStatus::Intermediate: status = QStringLiteral("�"); break;
		case FEStatus::Quazielementary: status = QStringLiteral("��"); break;
		default:
			status = "hz =)";
			break;
		}
		QString _form;
		switch (elements[i]->getFormalization())                                //������� ������������
		{
		case FEFormalization::Algorithmic: _form = QStringLiteral("��."); break;
		case FEFormalization::Analytical: _form = QStringLiteral("��."); break;
		case FEFormalization::Empirical: _form = QStringLiteral("�"); break;
		case FEFormalization::Information: _form = QStringLiteral("�"); break;
		case FEFormalization::Statistical: _form = QStringLiteral("�"); break;

		default:
			_form = "hz =)";
			break;
		}
		row[2] = new QStandardItem(status);
		row[3] = new QStandardItem(_form);
		form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3] }));
	}
	fillForm(formB, form);

}

void PM_spec_widget::generateFormD()
{
	
	QStandardItemModel form;
	AbstractModel *am = IS::getPrimaryModel();

	PrimaryModel *pm = (PrimaryModel*)am;

	vector<AFElement*> pdvec;
	for (int i = 0; i < pm->getSOS()->getElementsCount(); ++i)
	{
		AFElement *e = pm->getSOS()->getElement(i);
		if (e->getStatus() == Elementary)
			pdvec.push_back(e);
	}
	if (formDsettings->ui.comboBox->currentIndex() == 0)
		sort(pdvec.begin(), pdvec.end(), AFElement::AFEcodeOrder);
	else
		sort(pdvec.begin(), pdvec.end(), AFElement::AFEcompOrder);



	form.setColumnCount(6);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("��� ��"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("������������"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("����."));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("�������"));
	form.setHeaderData(4, Qt::Horizontal, QStringLiteral("���������"));
	form.setHeaderData(5, Qt::Horizontal, QStringLiteral("����������"));


	for (auto it = pdvec.begin(); it != pdvec.end(); ++it)
	{
		AFElement *e = *it;
		QString code = QString::fromWCharArray(e->getCode().data());
		QString name = QString::fromWCharArray(e->getName().data());
		QString mark = QString::fromWCharArray(((SubjectOperation*)e)->getMark().data());
		QString _form;
		QString args;
		QString func;
		switch (e->getFormalization())
		{
		case Analytical: _form = QStringLiteral("��"); break;
		case Statistical: _form = QStringLiteral("��"); break;
		case Empirical: _form = QStringLiteral("��"); break;
		default:
			break;
		}

		vector<Parameter*> argsvec;
		vector<Parameter*> defagrvec;
		Parameter *funcP = 0;
		ALElementList list = e->getBinaryLinksList();
		for (int i = 0; i < list.count(); ++i)
		{
			AEBinaryLink *l = list.element<AEBinaryLink *>(i);
			if (l->getType() == LT_CONTENT)
			{
				AEContentLink *cl = (AEContentLink*)l;
				if (cl->getChar() == '+')
					argsvec.push_back((Parameter*)cl->getAIelement());
				else if (cl->getChar() == '.')
					defagrvec.push_back((Parameter*)cl->getAIelement());
				else if (cl->getChar() == '*')
					funcP = (Parameter*)cl->getAIelement();
			}
		}
		sort(argsvec.begin(), argsvec.end(), &ParametersList::sortParams);
		sort(defagrvec.begin(), defagrvec.end(), &ParametersList::sortParams);
		for (auto it = argsvec.begin(); it != argsvec.end(); ++it)
		{
			Parameter *p = *it;
			QString name = QString::fromWCharArray(p->getName().data()).simplified();
			QString sign = QString::fromWCharArray(p->getSign().data()).simplified();
			if (sign != "")
				name += ", " + sign;
			args += "-" + name + "<br>";
		}
		for (auto it = defagrvec.begin(); it != defagrvec.end(); ++it)
		{
			Parameter *p = *it;
			QString name = QString::fromWCharArray(p->getName().data()).simplified();
			QString sign = QString::fromWCharArray(p->getSign().data()).simplified();
			if (sign != "")
				name += ", " + sign;
			if ((name = name.simplified()) != "")
				args += "-/" + name + "<br>";
		}
		ALElementList autoArgs = e->getAutoArgs('.');
		for (int i = 0; i < autoArgs.count(); ++i)
		{
			Parameter *p = autoArgs.element<Parameter *>(i);
			QString name = QString::fromWCharArray(p->getName().data()).simplified();
			QString sign = QString::fromWCharArray(p->getSign().data()).simplified();
			if (sign != "")
				name += ", " + sign;
			if ((name = name.simplified()) != "")
				args += "-/" + name + "<br>";
		}
		if (funcP)
		{
			QString name = QString::fromWCharArray(funcP->getName().data()).simplified();
			QString sign = QString::fromWCharArray(funcP->getSign().data()).simplified();
			if (sign != "")
				name += ", " + sign;
			func = name;
		}

		QStandardItem *row[6];
		row[0] = new QStandardItem(code);
		row[1] = new QStandardItem(name);
		row[1]->setData("text-align: left;", Qt::UserRole + 1);
		row[2] = new QStandardItem(_form);
		row[3] = new QStandardItem(func);
		row[3]->setData("text-align: left;", Qt::UserRole + 1);
		row[4] = new QStandardItem(args);
		row[4]->setData("text-align: left;", Qt::UserRole + 1);
		row[5] = new QStandardItem(mark); 
		row[5]->setData("text-align: left; white-space: pre-wrap;", Qt::UserRole + 1);
		form.appendRow(QList<QStandardItem*>({ row[0],row[1], row[2], row[3], row[4], row[5] }));
		
	}


	fillForm(formD,form);
	
}

void PM_spec_widget::generateFormD1()
{
	vector<Parameter *> params;
	PrimaryModel *pm = (PrimaryModel*)IS::getPrimaryModel();

	for (int i = 0; i < pm->getParamsList()->getParamsCount(); ++i)
	{
		params.push_back(pm->getParamsList()->getParameter(i));
	}

	sort(params.begin(), params.end(), &ParametersList::sortParams);


	QStandardItemModel form;

	form.setColumnCount(4);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("���"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("������������"));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("�����������"));
	form.setHeaderData(3, Qt::Horizontal, QStringLiteral("������"));

	for (int i = 0; i < params.size(); ++i)
	{
		Parameter *p = params[i];
		
		QStandardItem *row[4];
		
		row[0] = new QStandardItem(QString::fromWCharArray(p->getCode().data()));
		row[1] = new QStandardItem(QString::fromWCharArray(p->getName().data()));
		row[1]->setData("text-align: left;", Qt::UserRole + 1);
		row[2] = new QStandardItem(QString::fromWCharArray(p->getSign().data()));

		if (p->getObject())
		{
			row[3] = new QStandardItem(QString::fromWCharArray(p->getObject()->getName().data()));
		}
		else
		{
			row[3] = new QStandardItem("");
		}

		form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2],row[3] }));

	}
	fillForm(formD1, form);
}

void PM_spec_widget::generateFormD2()
{



	QStandardItemModel form;

	form.setColumnCount(3);
	form.setHeaderData(0, Qt::Horizontal, QStringLiteral("��� ��"));
	form.setHeaderData(1, Qt::Horizontal, QStringLiteral("��� �����."));
	form.setHeaderData(2, Qt::Horizontal, QStringLiteral("���� �����."));

	PrimaryModel *pm = (PrimaryModel*)IS::getPrimaryModel();

	vector<AFElement*> pdvec;
	vector<Parameter*> pvec;
	for (int i = 0; i < pm->getSOS()->getElementsCount(); ++i)
	{
		AFElement *e = pm->getSOS()->getElement(i);
		if (e->getStatus() == Elementary)
			pdvec.push_back(e);
	}
	sort(pdvec.begin(), pdvec.end(), AFElement::AFEcompOrder);

	for (auto it = pdvec.begin(); it != pdvec.end(); ++it)
	{
		AFElement *e = *it;
		QString pd = QString::fromWCharArray(e->getCode().data());
		QStandardItem *row[3];

		Parameter *func = (Parameter *)e->getFunction();
		if (func)
		{
			QString p = QString::fromWCharArray(func->getCode().data());

			row[0] = new QStandardItem(pd);
			row[1] = new QStandardItem(p);
			row[2] = new QStandardItem(QStringLiteral("�������"));

			form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2]}));
		}

		ALElementList args1 = e->getContentArgs('+');
		for (int i = 0; i < args1.count(); ++i)
		{
			pvec.push_back(args1.element<Parameter*>(i));
		}
		sort(pvec.begin(), pvec.end(), ParametersList::sortParams);
		foreach(Parameter *par, pvec)
		{

			QString p = QString::fromWCharArray(par->getCode().data());

			row[0] = new QStandardItem(pd);
			row[1] = new QStandardItem(p);
			row[2] = new QStandardItem(QStringLiteral("��������"));

			form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2] }));
		}
		pvec.clear();

		ALElementList args2 = e->getContentArgs('.');
		for (int i = 0; i < args2.count(); ++i)
		{
			pvec.push_back(args2.element<Parameter*>(i));
		}
		sort(pvec.begin(), pvec.end(), ParametersList::sortParams);
		foreach(Parameter *par, pvec)
		{

			QString p = QString::fromWCharArray(par->getCode().data());

			row[0] = new QStandardItem(pd);
			row[1] = new QStandardItem(p);
			row[2] = new QStandardItem(QStringLiteral("�������� �� ���������"));

			form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2] }));
		}
		pvec.clear();

		ALElementList args3 = e->getAutoArgs('.');
		for (int i = 0; i < args3.count(); ++i)
		{
			pvec.push_back(args3.element<Parameter*>(i));
		}
		sort(pvec.begin(), pvec.end(), ParametersList::sortParams);
		foreach(Parameter *par, pvec)
		{

			QString p = QString::fromWCharArray(par->getCode().data());

			row[0] = new QStandardItem(pd);
			row[1] = new QStandardItem(p);
			row[2] = new QStandardItem(QStringLiteral("�������� �� ���������"));
			form.appendRow(QList<QStandardItem*>({ row[0],row[1],row[2] }));
		}
		pvec.clear();
	}


	fillForm(formD2, form);
}
