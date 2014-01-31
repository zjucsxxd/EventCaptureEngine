#include "adddictionary.h"
#include <QLocale>
#include <QVariant>
#include <QFileDialog>
#include "dictionary.h"
AddDictionary::AddDictionary(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	for (int i = QLocale::C + 1; i < QLocale::LastLanguage; ++i) 
	{
		QString lang = QLocale::languageToString(QLocale::Language(i));
		ui.cmbDictionary->addItem(lang, QVariant(i));
	}
	connect(ui.cmbDictionary,	SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbDictionary_Changed(int)));
	connect(ui.btnBrowse, SIGNAL(clicked()),this,SLOT(btnBrowse_Clicked()));
	connect(ui.btnOk,     SIGNAL(clicked()),this,SLOT(btnOk_Clicked()));

	ui.btnOk->setEnabled(false);
}

static void fillCountryCombo(const QVariant &lng, QComboBox *combo)
{
	combo->clear();
	QLocale::Language lang = QLocale::Language(lng.toInt());
	if (lang != QLocale::C) 
	{
		foreach (QLocale::Country cntr, QLocale::countriesForLanguage(lang)) 
		{
			QString country = QLocale::countryToString(cntr);
			combo->addItem(country, QVariant(cntr));
		}
		combo->model()->sort(0, Qt::AscendingOrder);
	}
	combo->insertItem(0, AddDictionary::tr("Any Country"), QVariant(QLocale::AnyCountry));
	combo->setCurrentIndex(0);
}

void AddDictionary::cmbDictionary_Changed(int idx)
{
	fillCountryCombo(ui.cmbDictionary->itemData(idx), ui.cmbCountry);
}
void AddDictionary::btnBrowse_Clicked()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",
		"Translation files (*.dic);;");

	ui.txtDicPath->setText(path);

	ui.btnOk->setEnabled(true);
}

void AddDictionary::btnOk_Clicked()
{
	Dictionary *e=static_cast<Dictionary*>(this->parent());
	e->loadDictionary(ui.txtDictionaryName->text(),ui.cmbDictionary->currentText(),ui.cmbCountry->currentText(),ui.txtDicPath->text());

	ui.txtDictionaryName->clear();
	ui.txtDicPath->clear();

}
AddDictionary::~AddDictionary()
{

}
