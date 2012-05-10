/*
 * 
 *  Copyright (c) 2011
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cryptfiles.h"
#include "miscfunctions.h"
#include "../zuluCrypt-cli/constants.h"

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <iostream>
#include <QFile>
#include <QKeyEvent>

cryptfiles::cryptfiles(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::cryptfiles)
{
	m_ui->setupUi(this);
	this->setWindowFlags(Qt::Window | Qt::Dialog);
	this->setFont(parent->font());
	this->setFixedSize(this->size());

	m_msg.setParent(this);

	m_ui->pbOpenFolder->setIcon(QIcon(QString(":/folder.png")));
	m_ui->pushButtonFile->setIcon(QIcon(QString(":/file.png")));

	connect(m_ui->pbCreate,SIGNAL(clicked()),this,SLOT(pbCreate())) ;
	connect(m_ui->pushButtonFile,SIGNAL(clicked()),this,SLOT(pbOpenFile()));
	connect(m_ui->pbOpenFolder,SIGNAL(clicked()),this,SLOT(pbOpenFolder()));
	connect(m_ui->rbKey,SIGNAL(clicked()),this,SLOT(rbKeyChecked()));
	connect(m_ui->rbKeyFile,SIGNAL(clicked()),this,SLOT(rbKeyFileChecked()));
	connect(m_ui->lineEditSourcePath,SIGNAL(textChanged(QString)),this,SLOT(sourceTextChanged(QString)));
	connect(m_ui->pushButtonKeyFile,SIGNAL(clicked()),this,SLOT(pbKeyFile()));
	connect(m_ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(pbCancel()));

	this->rbKeyChecked() ;

	m_OperationInProgress = false ;

	m_ui->rbKey->setChecked(true);

	m_ui->lineEditDestinationPath->setText(QDir::homePath() + QString("/"));
	m_ui->lineEditPass_2->setEchoMode(QLineEdit::Password);

	m_ui->lineEditSourcePath->setFocus();

}

void cryptfiles::rbKeyChecked()
{
	m_ui->pushButtonKeyFile->setIcon(QIcon(QString(":/passphrase.png")));
	m_ui->pushButtonKeyFile->setEnabled(false);
	m_ui->lineEditPass_1->clear();
	m_ui->lineEditPass_2->clear();
	m_ui->lineEditPass_1->setEchoMode(QLineEdit::Password);	
	m_ui->lineEditPass_1->setFocus();

	if(m_operation == QString("-E")){
		m_ui->labelKey2->setEnabled(true);
		m_ui->lineEditPass_2->setEnabled(true);
	}

}

void cryptfiles::rbKeyFileChecked()
{
	m_ui->pushButtonKeyFile->setIcon(QIcon(QString(":/keyfile.png")));
	m_ui->lineEditPass_2->setEnabled(false);
	m_ui->pushButtonKeyFile->setEnabled(true);
	m_ui->labelKey2->setEnabled(false);
	m_ui->lineEditPass_1->clear();
	m_ui->lineEditPass_2->clear();
	m_ui->lineEditPass_1->setEchoMode(QLineEdit::Normal);
	m_ui->lineEditPass_1->setFocus();

}

void cryptfiles::sourceTextChanged(QString source)
{
	QString dest ;

	if(m_operation == QString("-E"))
		dest = source.split("/").last() + QString(".zc");
	else
		dest = source.split("/").last() ;

	QStringList p = m_ui->lineEditDestinationPath->text().split("/");

	int size = p.size() ;
	QString path = QString("");
	for( int i = 0 ; i < size - 1 ; i++ )
		path += p.at(i) + QString("/") ;

	path += dest ;

	if(m_operation == QString("-D"))
		if(path.endsWith(QString(".zc")))
			path = path.mid(0,path.size() - 3) ;

	m_ui->lineEditDestinationPath->setText(path);

}

void cryptfiles::encrypt()
{
	m_operation = QString("-E") ;
	this->setWindowTitle(tr("create encrypted version of a file"));
	this->show();
}

void cryptfiles::decrypt()
{
	m_operation = QString("-D");
	m_ui->labelKey2->setEnabled(false);
	m_ui->lineEditPass_2->setEnabled(false);
	this->setWindowTitle(tr("create decrypted version of an encrypted file"));
	this->show();
}

void cryptfiles::closeEvent(QCloseEvent *e)
{
	e->ignore();
	this->HideUI();

}

void cryptfiles::pbCancel()
{
	this->HideUI();
}

void cryptfiles::HideUI()
{
	if(m_OperationInProgress)
		return ;
	emit this->HideUISignal();
	this->hide();
}

void cryptfiles::enableAll()
{
	if(m_operation == QString("-E")){
		m_ui->labelKey2->setEnabled(true);
		m_ui->lineEditPass_2->setEnabled(true);
	}

	m_ui->labelKey->setEnabled(true);
	m_ui->lineEditPass_1->setEnabled(true);

	m_ui->labelDestinationPath->setEnabled(true);
	m_ui->labelSourcePath->setEnabled(true);
	m_ui->lineEditDestinationPath->setEnabled(true);
	m_ui->lineEditSourcePath->setEnabled(true);
	m_ui->pbCreate->setEnabled(true);
	m_ui->pbOpenFolder->setEnabled(true);
	m_ui->pushButtonFile->setEnabled(true);
	m_ui->pushButtonCancel->setEnabled(true);
	m_ui->rbKey->setEnabled(true);
	m_ui->rbKeyFile->setEnabled(true);
}

void cryptfiles::disableAll()
{
	m_ui->lineEditPass_1->setEnabled(false);
	m_ui->lineEditPass_2->setEnabled(false);
	m_ui->labelKey2->setEnabled(false);
	m_ui->labelKey->setEnabled(false);
	m_ui->pushButtonCancel->setEnabled(false);
	m_ui->labelDestinationPath->setEnabled(false);
	m_ui->labelSourcePath->setEnabled(false);
	m_ui->lineEditDestinationPath->setEnabled(false);
	m_ui->lineEditSourcePath->setEnabled(false);
	m_ui->pbCreate->setEnabled(false);
	m_ui->pbOpenFolder->setEnabled(false);
	m_ui->pushButtonFile->setEnabled(false);
	m_ui->rbKey->setEnabled(false);
	m_ui->rbKeyFile->setEnabled(false);

}

void cryptfiles::pbCreate()
{
	QString source = miscfunctions::resolveHomeSymbol(m_ui->lineEditSourcePath->text());

	if( source.isEmpty() )
		return m_msg.UIMessage(tr("ERROR!"),tr("path to source field is empty"));

	QString dest = m_ui->lineEditDestinationPath->text();

	QString key_1 = m_ui->lineEditPass_1->text() ;
	QString key_2 = m_ui->lineEditPass_2->text() ;

	QString keySource ;
	if( m_ui->rbKey->isChecked()){
		if( key_1.isEmpty())
			return m_msg.UIMessage(tr("ERROR!"),tr("first key field is empty"));

		if(m_operation == QString("-E")){
			if( key_2.isEmpty())
				return m_msg.UIMessage(tr("ERROR!"),tr("second key field is empty"));
			if( key_1 != key_2)
				return m_msg.UIMessage(tr("ERROR!"),tr("keys do not match"));
		}
		keySource = QString("-p") ;
	}else{
		if(miscfunctions::exists(key_1) == false)
			return m_msg.UIMessage(tr("ERROR!"),tr("invalid path to key file"));

		keySource = QString("-f") ;
	}

	source.replace("\"","\"\"\"") ;
	dest.replace("\"","\"\"\"") ;
	key_1.replace("\"","\"\"\"") ;

	QString	exe = QString("%1 %2 -d \"%3\" -e \"%4\" %5 \"%6\"").arg(ZULUCRYPTzuluCrypt).arg(m_operation).arg(source).arg(dest).arg(keySource).arg(key_1);
	
	this->disableAll();

	m_OperationInProgress = true ;

	runInThread * rt = new runInThread(exe);
	connect(rt,SIGNAL(finished(int)),this,SLOT(threadExitStatus(int)));
	rt->start();

	disableAll() ;
}

void cryptfiles::pbOpenFolder(void)
{
	QString p = tr("Select Path to put destination file") ;
	QString Z = QFileDialog::getExistingDirectory(this,p,QDir::homePath(),QFileDialog::ShowDirsOnly) ;

	if(Z.isEmpty())
		Z = QDir::homePath() ;
	QString path ;
	if(m_operation == QString("-E"))
		path = Z + QString("/") + m_ui->lineEditSourcePath->text().split("/").last() + QString(".zc");
	else{
		path = Z + QString("/") + m_ui->lineEditSourcePath->text().split("/").last();
	}

	m_ui->lineEditDestinationPath->setText( path );
	if(m_ui->lineEditSourcePath->text().isEmpty() == true)
		m_ui->lineEditSourcePath->setFocus();
	else
		m_ui->pbCreate->setFocus();
}

void cryptfiles::pbOpenFile()
{
	QString Z = QFileDialog::getOpenFileName(this,tr("select a file you want to work on"),QDir::homePath(),0);
	m_ui->lineEditSourcePath->setText(Z);
	m_ui->lineEditPass_1->setFocus();
}

void cryptfiles::pbKeyFile()
{
	QString Z = QFileDialog::getOpenFileName(this,tr("select a key file"),QDir::homePath(),0);

	m_ui->lineEditPass_1->setText(Z);
	if(m_ui->lineEditSourcePath->text().isEmpty())
		m_ui->lineEditSourcePath->setFocus();
	else
		m_ui->pbCreate->setFocus();
}

void cryptfiles::threadExitStatus(int st)
{
	m_OperationInProgress = false ;

	switch( st ){
		case 0 : m_msg.UIMessage( tr("SUCCESS"),tr("encrypted file created successfully" ) ) ;
			 return this->HideUI();
		case 1 : m_msg.UIMessage( tr("SUCCESS"),tr("decrypted file created successfully" ) )	;
			 return this->HideUI();
		case 2 : m_msg.UIMessage( tr("ERROR!"),tr("could not open key file for reading" ) )				; break ;
		case 3 : m_msg.UIMessage( tr("ERROR!"),tr("missing key source" ) )						; break ;
		case 4 : m_msg.UIMessage( tr("ERROR!"),tr("could not open encryption routines" ) )				; break ;
		case 5 : m_msg.UIMessage( tr("INFO!"),tr("file or folder already exist at destination address" ) )		; break ;
		case 6 : m_msg.UIMessage( tr("ERROR!"),tr("invalid path to source" ))						; break ;
		case 7 : m_msg.UIMessage( tr("ERROR!"),tr("could not resolve path to destination file" ))			; break ;
		case 8 : m_msg.UIMessage( tr("ERROR!"),tr("passphrases do not match" ))						; break ;
		case 9 : m_msg.UIMessage( tr("ERROR!"),tr("required argument is missing" ) )					; break ;
		case 10: m_msg.UIMessage( tr("ERROR!"),tr("insufficient privilege to create destination file" ))		; break ;
		case 11: m_msg.UIMessage( tr("ERROR!"),tr("wrong passphrase" ))						 	; break ;
	}
	this->enableAll();
	if( st == 11 || st == 2 ){
		m_ui->lineEditPass_1->clear();
		m_ui->lineEditPass_1->setFocus();
	}
}

cryptfiles::~cryptfiles()
{
	delete m_ui;
}